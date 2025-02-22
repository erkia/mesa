// Copyright 2024 Valve Corporation
// SPDX-License-Identifier: MIT

#ifdef VULKAN
#define CONFIG(_name, _id, default_val) layout (constant_id = _id) const int _name = default_val;
#else
enum rs_config {
#define CONFIG(_name, _id, default_val) _name = _id,
#endif

#define RS_FILL_WORKGROUP_SIZE_ID 0
CONFIG(RS_FILL_WORKGROUP_SIZE, RS_FILL_WORKGROUP_SIZE_ID, 7)
CONFIG(RS_FILL_BLOCK_ROWS, 1, 8)

#define RS_HISTOGRAM_WORKGROUP_SIZE_ID 2
CONFIG(RS_HISTOGRAM_WORKGROUP_SIZE, RS_HISTOGRAM_WORKGROUP_SIZE_ID, 7)
CONFIG(RS_HISTOGRAM_SUBGROUP_SIZE_LOG2, 3, 7)
CONFIG(RS_HISTOGRAM_BLOCK_ROWS, 4, 8)

#define RS_PREFIX_WORKGROUP_SIZE_ID 5
CONFIG(RS_PREFIX_WORKGROUP_SIZE, RS_PREFIX_WORKGROUP_SIZE_ID, 8)
CONFIG(RS_PREFIX_SUBGROUP_SIZE_LOG2, 6, 6)

#define RS_SCATTER_WORKGROUP_SIZE_ID 7
CONFIG(RS_SCATTER_WORKGROUP_SIZE, RS_SCATTER_WORKGROUP_SIZE_ID, 8)
CONFIG(RS_SCATTER_SUBGROUP_SIZE_LOG2, 8, 6)
CONFIG(RS_SCATTER_BLOCK_ROWS, 9, 14)

CONFIG(RS_SCATTER_NONSEQUENTIAL_DISPATCH, 10, 0)

#ifndef VULKAN
};
#endif
