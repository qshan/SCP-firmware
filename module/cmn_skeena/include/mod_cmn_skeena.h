/*
 * Arm SCP/MCP Software
 * Copyright (c) 2017-2021, Arm Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef MOD_CMN_SKEENA_H
#define MOD_CMN_SKEENA_H

#include <fwk_id.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/*!
 * \addtogroup GroupModules Modules
 * \{
 */

/*!
 * \defgroup GroupModuleCMN_SKEENA CMN_SKEENA
 *
 * \brief Arm Coherent Mesh Network (CMN) Skeena module
 *
 * \details This module adds support for the CMN_SKEENA interconnect
 * \{
 */

/*!
 * \brief Max entries of the Memory Map table
 *        for the Home Agent.
 */
#define MAX_HA_MMAP_ENTRIES 4

/*!
 * \brief Module API indices
 */
enum mod_cmn_skeena_api_idx {
    /*! Index of the PPU_V1 power state observer API */
    MOD_CMN_SKEENA_API_IDX_PPU_OBSERVER,

    /*! Index of the CCIX config setup API */
    MOD_CMN_SKEENA_API_IDX_CCIX_CONFIG,

    /*! Number of APIs */
    MOD_CMN_SKEENA_API_COUNT
};

/*!
 * \brief Memory region configuration type
 */
enum mod_cmn_skeena_memory_region_type {
    /*! Input/Output region (serviced by dedicated HN-I and HN-D nodes) */
    MOD_CMN_SKEENA_MEMORY_REGION_TYPE_IO,

    /*!
     * Region backed by the system cache (serviced by all HN-F nodes in the
     * system)
     */
    MOD_CMN_SKEENA_MEMORY_REGION_TYPE_SYSCACHE,

    /*!
     * Sub region of the system cache for non-hashed access (serviced by
     * HN-F nodes).
     */
    MOD_CMN_SKEENA_REGION_TYPE_SYSCACHE_SUB,

    /*!
     * Region used for CCIX access.
     */
    MOD_CMN_SKEENA_REGION_TYPE_CCIX,

    /*!
     * Non-hash regions in SYSCACHE group.
     */
    MOD_CMN_SKEENA_REGION_TYPE_SYSCACHE_NONHASH,
};

/*!
 * \brief Memory region map descriptor
 */
struct mod_cmn_skeena_memory_region_map {
    /*! Base address */
    uint64_t base;

    /*! Region size in bytes */
    uint64_t size;

    /*! Region configuration type */
    enum mod_cmn_skeena_memory_region_type type;

    /*!
     * \brief Target node identifier
     *
     * \note Not used for ::MOD_CMN_SKEENA_MEMORY_REGION_TYPE_SYSCACHE memory
     *      regions as it uses the pool of HN-F nodes available in the system.
     */
    unsigned int node_id;
};

/*!
 * \brief HN-I region attributes
 */
/*!
 * Used to map a given address region to physical memory
 */
#define CMN_SKEENA_HNI_REG_PHY_MEM_EN UINT64_C(0x0400000000000000)
/*!
 * Used to serialize all writes within a given address region
 */
#define CMN_SKEENA_HNI_REG_SER_ALL_WR UINT64_C(0x0800000000000000)
/*!
 * Used to serialize Device-nGnRnE writes within a given address region
 */
#define CMN_SKEENA_HNI_REG_SER_DEVNE_WR UINT64_C(0x1000000000000000)
/*!
 * Enables sending early read receipts from HN-I in a given address region
 */
#define CMN_SKEENA_HNI_REG_EARLY_RDACK_EN UINT64_C(0x2000000000000000)
/*!
 * Enables early write acknowledgement in a given address region
 */
#define CMN_SKEENA_HNI_REG_EARLY_WRCOMP_EN UINT64_C(0x4000000000000000)
/*!
 * Indicates whether a given address region's fields are programmed and valid
 */
#define CMN_SKEENA_HNI_REG_VALID UINT64_C(0x8000000000000000)

/*!
 * \brief RN-I Configuration
 */
/*!
 * 'PCIe Root Present' bit position
 */
#define CMN_SKEENA_RNI_PCIE_MSTR_POS 5
/*!
 * Indicates whether a PCIe Root is present upstream of RNI
 */
#define CMN_SKEENA_RNI_PCIE_MSTR_PRE \
    (UINT64_C(1) << CMN_SKEENA_RNI_PCIE_MSTR_POS)

/*!
 * \brief HN-I Memory region map descriptor
 */
struct mod_cmn_skeena_hni_region_map {
    /*! Base address */
    uint64_t base;

    /*! Region size in bytes */
    uint64_t size;

    /*! Region configuration flags */
    uint64_t flags;

    /*! HN-I region node ID to which this region to be configured */
    unsigned int node_id;

    /*! Region number to be programmed */
    uint8_t region_num;
};

/*!
 * \brief CMN_SKEENA configuration data
 */
struct mod_cmn_skeena_config {
    /*! Peripheral base address. */
    uintptr_t base;

    /*! Size along x-axis of the interconnect mesh */
    unsigned int mesh_size_x;

    /*! Size along y-axis of the interconnect mesh */
    unsigned int mesh_size_y;

    /*! Default HN-D node identifier containing the global configuration */
    unsigned int hnd_node_id;

    /*!
     * \brief Table of SN-Fs used as targets for the HN-F nodes
     *
     * \details Each entry of this table corresponds to a HN-F node in the
     *      system. The HN-F's logical identifiers are used as indices in this
     *      table
     */
    const unsigned int *snf_table;

    /*! Number of entries in the ::mod_cmn_skeena_config::snf_table */
    size_t snf_count;

    /*! Host SA count */
    unsigned int sa_count;

    /*! Table of region memory map entries */
    const struct mod_cmn_skeena_memory_region_map *mmap_table;

    /*! Number of entries in the ::mod_cmn_skeena_config::mmap_table */
    size_t mmap_count;

    /*! Table of RN-I node IDs with PCIe master connected */
    const unsigned int *rni_pcie_table;

    /*! Number of entries in the ::mod_cmn_skeena_config::rni_pcie_table */
    size_t rni_pcie_count;

    /*! Table of HN-I region memory map entries */
    const struct mod_cmn_skeena_hni_region_map *hni_mmap_table;

    /*! Number of entries in the ::mod_cmn_skeena_config::hni_mmap_table */
    size_t hni_mmap_count;

    /*! Address space of the chip */
    uint64_t chip_addr_space;

    /*! Identifier of the clock that this device depends on */
    fwk_id_t clock_id;

    /*!
     * \brief HN-F with CAL support flag
     * \details When set to true, enables HN-F with CAL support. This flag will
     *      be used only if HN-F is found to be connected to CAL (When connected
     *      to a CAL port, node id of HN-F will be a odd number).
     */
    bool hnf_cal_mode;
};

/*!
 * \brief CCIX HA memory table entry structure
 */
struct mod_cmn_skeena_ccix_ha_mmap {
    /*! HA node ID */
    uint8_t ha_id;

    /*! Base address */
    uint64_t base;

    /*! Region size in bytes */
    uint64_t size;
};

/*!
 * \brief CMN_SKEENA CCIX configuration data from remote node
 */
struct mod_cmn_skeena_ccix_remote_node_config {
    /*!
     * Count of remote caching agent (RN-F) that can send request to local HNs
     */
    uint8_t remote_rnf_count;

    /*! Remote SA count */
    uint8_t remote_sa_count;

    /*! Remote HA count */
    uint8_t remote_ha_count;

    /*! PCIe traffic class used for CCIX Virtual Channel */
    uint8_t ccix_tc;

    /*! CCIX message packing flag */
    bool ccix_msg_pack_enable;

    /*! PCIe bus number on which CCIX link is enabled */
    uint8_t pcie_bus_num;

    /*! CCIX link identifier */
    uint8_t ccix_link_id;

    /*! optimised tlp mode */
    bool ccix_opt_tlp;

    /*! Remote HA memory map table count */
    uint8_t remote_ha_mmap_count;

    /*! SMP mode */
    bool smp_mode;

    /*! Remote HA memory map table */
    struct mod_cmn_skeena_ccix_ha_mmap remote_ha_mmap[MAX_HA_MMAP_ENTRIES];

    /*! Max Packet Size */
    uint8_t ccix_max_packet_size;
};

/*!
 * \brief CMN_SKEENA CCIX configuration data from host node
 */
struct mod_cmn_skeena_ccix_host_node_config {
    /*! Host RA count */
    uint8_t host_ra_count;

    /*! Host SA count */
    uint8_t host_sa_count;

    /*! Host HA count */
    uint8_t host_ha_count;

    /*! CCIX HA memory map table count for endpoints */
    uint8_t ccix_host_mmap_count;

    /*! CCIX HA memory map table for endpoints */
    struct mod_cmn_skeena_ccix_ha_mmap ccix_host_mmap[MAX_HA_MMAP_ENTRIES];

    /*! CCIX Maximum Memory Request send credits from Host */
    uint16_t ccix_request_credits;

    /*! CCIX Maximum Snoop send credits from Host */
    uint16_t ccix_snoop_credits;

    /*! CCIX Maximum Data send credits from Host */
    uint16_t ccix_data_credits;

    /*! Max Packet Size */
    uint8_t ccix_max_packet_size;

    /*! CCIX optimised tlp mode capabiltiy of Host */
    bool ccix_opt_tlp;

    /*! CCIX message packing flag capability of Host */
    bool ccix_msg_pack_enable;
};

/*!
 * \brief CMN_SKEENA CCIX configuration interface
 */
struct mod_cmn_skeena_ccix_config_api {
    /*!
     * \brief Get the CCIX host configuration
     *
     * \param[out] config CCIX host configuration
     *
     * \retval ::FWK_SUCCESS if the operation succeed.
     * \return one of the error code otherwise.
     */
    int (*get_config)(struct mod_cmn_skeena_ccix_host_node_config *config);
    /*!
     * \brief set the CCIX endpoint configuration
     *
     * \param[in] config CCIX endpoint configuration
     *
     * \retval ::FWK_SUCCESS if the operation succeed.
     * \return one of the error code otherwise.
     */
    int (*set_config)(struct mod_cmn_skeena_ccix_remote_node_config *config);
    /*!
     * \brief Interface to trigger the protocol credit exchange
     *
     * \param  link_id Link on which the protocol credit exchange
     *                 would initiate.
     *
     * \retval ::FWK_SUCCESS if the operation succeed.
     * \return one of the error code otherwise.
     */
    int (*exchange_protocol_credit)(uint8_t link_id);
    /*!
     * \brief Interface to configure for system coherency
     *
     * \param  link_id Link on which the coherency has to
     *                 be enabled.
     *
     * \retval ::FWK_SUCCESS if the operation succeed.
     * \return one of the error code otherwise.
     */
    int (*enter_system_coherency)(uint8_t link_id);
    /*!
     * \brief Interface to enter DVM domain
     *
     * \param  link_id Link on which DVM domain has to be enabled
     *
     * \retval ::FWK_SUCCESS if the operation succeed.
     * \return one of the error code otherwise.
     */
    int (*enter_dvm_domain)(uint8_t link_id);
};

/*!
 * \}
 */

/*!
 * \}
 */

#endif /* MOD_CMN_SKEENA_H */
