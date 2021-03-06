#include <libaio.h>
#include "libaio_device.h"
#include "libaio_device_server.h"
#include "../log/include/awe_log.h"

#define USE_LIBAIO

libaio_device::libaio_device(std::string dev_path, int max_event_num, libaio_device_service *device_service,
                             io_done_callback *io_cb):
        dev_path(dev_path),
        device_service(device_service),
        fd_ref(0),
        threads(4),
        io_cb_(io_cb) {
    pending_size = 0;
    int res = 0;
    libaio_context = (io_context_t *) malloc(sizeof(io_context_t));
    if ( !libaio_context )
    {
        AWE_MODULE_ERROR("aio", "alloc libaio_context failed");
        assert(libaio_context != NULL);
    }
    memset(libaio_context, 0, sizeof(io_context_t));
    res = io_setup(max_event_num, libaio_context);
    if ( res != 0 )
    {
        AWE_MODULE_ERROR("aio", "io set up failed : %d", res);
    }
    assert(res == 0);

    event_fd = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
    if ( event_fd == -1 )
    {
        AWE_MODULE_ERROR("aio", "create eventfd err");
        return;
    }
    AWE_MODULE_DEBUG("aio", "libaio_device : %p, "
                            "libaio_context : %p, path : %s", this, libaio_context,
                     dev_path.c_str());
}

int libaio_device::open()
{
    AWE_MODULE_DEBUG("aio", "libaio_device : %p, "
                            "libaio_context : %p, path : %s", this, libaio_context,
                            dev_path.c_str());
    int res = ::open(dev_path.c_str(), O_RDWR | O_DIRECT);
    if ( res == -1 )
    {
        AWE_MODULE_ERROR("aio", "aio open file err path : %s", dev_path.c_str());
        return -1;
    }

    dev_fd = (unsigned long) res;
    device_service->insert_device(this);
    return 0;
}

void libaio_device::close()
{
    AWE_MODULE_DEBUG("aio", "libaio_device : %p, "
                            "libaio_context : %p, path : %s", this, libaio_context,
                     dev_path.c_str());
    ::close(dev_fd);
}

void libaio_device::put_pending_size()
{
    --pending_size;
}

int libaio_device::obtain_pending_size()
{
    return pending_size;
}

int libaio_device::sync_read(unsigned long offset, unsigned int len, unsigned char *buf)
{
    unsigned long off = offset << 9;
    ssize_t this_len = 0;
    for ( ssize_t total_len = 0; total_len < len; total_len += this_len )
    {
        this_len = pread(dev_fd, &buf[total_len], len - total_len, off + total_len);
        if ( this_len <= 0 )
            return -1;
    }
    return 0;
}

int libaio_device::sync_write(unsigned long offset, unsigned int len, unsigned char *buf)
{
    unsigned long off = offset << 9;
    ssize_t this_len = 0;
    for ( ssize_t total_len = 0; total_len < len; total_len += this_len )
    {
        this_len = pwrite(dev_fd, &buf[total_len], len - total_len, off + total_len);
        if ( this_len <= 0 )
            return -1;
    }

    return 0;
}

void libaio_device::async_read(unsigned long offset, unsigned int len, unsigned char *buf, void *pri)
{
#ifdef USE_LIBAIO
    void *buf_tmp = NULL;
    assert(len % 512 == 0);
    //printf("libaio_device::async_write\n");

    struct iocb *iocb_p = (iocb *) malloc(sizeof(struct iocb));
    memset(iocb_p, 0, sizeof(sizeof(struct iocb)));
    unsigned long off = offset << 9;

    if ( posix_memalign(&buf_tmp, 512, len))
    {
        perror("posix_memalign");
    }
    io_prep_pread(iocb_p, dev_fd, buf_tmp, len, off);

    event_ctx *ctx = new event_ctx(pri, true, iocb_p, buf);
    iocb_p->data = ctx;

    io_set_eventfd(iocb_p, event_fd);
    int submit_num = io_submit(*libaio_context, 1, &iocb_p);
    assert(submit_num == 1);

#else
    threads.submit_task(boost::bind(&test_device::fake_async_read, this, offset, len, buf, pri));
#endif
}


void libaio_device::async_write(unsigned long offset, unsigned int len, unsigned char *buf, void *pri)
{
#ifdef USE_LIBAIO
    void *tmp_buf = NULL;
    assert(len % ALIGN_SIZE == 0);

    if ( posix_memalign(&tmp_buf, ALIGN_SIZE, len))
    {
        perror("posix_memalign");
    }

    memcpy(tmp_buf, buf, len);
    struct iocb *iocb_p = (iocb *) malloc(sizeof(struct iocb));
    memset(iocb_p, 0, sizeof(sizeof(struct iocb)));
    unsigned long off = offset << 9;
    //std::cout << "off:" << offset << std::endl;
    io_prep_pwrite(iocb_p, dev_fd, tmp_buf, len, off);

    event_ctx *ctx = new event_ctx(pri, false, iocb_p, buf);
    iocb_p->data = ctx;

    io_set_eventfd(iocb_p, event_fd);
    int submit_num = io_submit(*libaio_context, 1, &iocb_p);
    //std::cout << "submit_num" << submit_num << std::endl;
    assert(submit_num == 1);

#else
    threads.submit_task(boost::bind(&test_device::fake_async_write, this, offset, len, buf, pri));
#endif
}

std::string &libaio_device::obtain_dev_path()
{
    return dev_path;
}

void libaio_device::get_pending_size()
{
    ++pending_size;
}

int libaio_device::get_event_fd()
{
    return this->event_fd;
}

void libaio_device::get_io()
{
    uint64_t finished_aio;
    uint64_t i = 0;
    struct io_event event;
    struct timespec timeout = {5, 0};
    std::lock_guard<std::mutex> lock(mtx_);
    int ret = read(event_fd, &finished_aio, sizeof(finished_aio));
    AWE_MODULE_DEBUG("aio", "libaio_device : %p, "
                            "libaio_context : %p, path : %s", this, libaio_context,
                     dev_path.c_str());
    if (ret != sizeof(finished_aio))
    {
        AWE_MODULE_ERROR("aio", "read event failed : %d", ret);
        //FIXME  错误处理
        assert(ret <= 0);
        return ;
    }
    for ( i = 0; i < finished_aio; i++ )
    {
        assert(libaio_context != nullptr);
        int res = io_getevents(*libaio_context, 1, 1, &event, &timeout);
        int error = 0;
        if ( res < 1 )
        {
            // io timeout or get events error;
            error = -1;
        }

        event_ctx *ctx = (event_ctx *) event.data;
        if(ctx->is_read)              //将读出来的内容拷贝到request buf
        {
            memcpy(ctx->buf_ptr, event.obj->u.c.buf, event.obj->u.c.nbytes);
        }
        free(event.obj->u.c.buf);
        free(event.obj);
        if(ctx->is_read) {
            io_cb_->read_done(ctx->pri, error);
        } else {
            io_cb_->write_done(ctx->pri, error);
        }
        delete ctx;
    }
}

libaio_device::~libaio_device() {
    AWE_MODULE_DEBUG("aio", "libaio_device : %p, "
                            "libaio_context : %p, path : %s", this, libaio_context,
                     dev_path.c_str());
    ::close(event_fd);
    io_destroy(*libaio_context);
    free(libaio_context);
    libaio_context = nullptr;
}


