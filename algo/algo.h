#ifndef _ALGO_H
#define _ALGO_H
#include <map>
#include <vector>
using namespace std;

#include "algo_interface.h"
#include "awe_conf/env.h"
#include "algo_section.h"
#include "algo_sections.h"
#include "algo_range.h"
#include "block_request.h"
#include "request_ref.h"
#include "algo_nocopy_request_mapper.h"
#include "algo_block_open_ref.h"
#include "algo_block_io_super.h"
#include "algo_block_multi_super.h"
#include "algo_block_io_jbod_nosuper.h"
#include "algo_block_io_jbod.h"
#include "super_algo_device.h"
#include "algo_error.h"
#include "psync_device.h"
#include "psync_service.h"

#define secs_to_size(secs) (unsigned int)((secs)<<9)
#define size_to_secs(size) (unsigned long long)((size)>>9)

#endif