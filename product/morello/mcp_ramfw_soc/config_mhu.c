/*
 * Arm SCP/MCP Software
 * Copyright (c) 2021, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <morello_mcp_mhu.h>
#include <morello_mcp_mmap.h>

#include <mod_mhu.h>

#include <fwk_element.h>
#include <fwk_id.h>
#include <fwk_module.h>

#include <fmw_cmsis.h>

static const struct fwk_element
    mhu_element_table[MORELLO_MHU_DEVICE_IDX_COUNT + 1] = {
        [MORELLO_MHU_DEVICE_IDX_S_SCP] = {
            .name = "MHU_S_SCP",
            .sub_element_count = 1,
            .data = &((struct mod_mhu_device_config){
                .irq = MHU_SCP_SEC_IRQ,
                .in = MHU_SCP_TO_MCP_S,
                .out = MHU_MCP_TO_SCP_S,
            }),
        },
        [MORELLO_MHU_DEVICE_IDX_COUNT] = { 0 },
};

static const struct fwk_element *mhu_get_element_table(fwk_id_t module_id)
{
    return mhu_element_table;
}

struct fwk_module_config config_morello_mhu = {
    .elements = FWK_MODULE_DYNAMIC_ELEMENTS(mhu_get_element_table),
};
