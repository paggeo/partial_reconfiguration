create_pblock pblock_reconfigurable_logic
create_pblock pblock_reconfigurable_logic_1
add_cells_to_pblock [get_pblocks pblock_reconfigurable_logic_1] [get_cells -quiet [list reconfigurable_logic]]
resize_pblock [get_pblocks pblock_reconfigurable_logic_1] -add {SLICE_X1Y339:SLICE_X9Y355}
resize_pblock [get_pblocks pblock_reconfigurable_logic_1] -add {DSP48E2_X0Y136:DSP48E2_X0Y141}
resize_pblock [get_pblocks pblock_reconfigurable_logic_1] -add {RAMB18_X0Y136:RAMB18_X0Y141}
resize_pblock [get_pblocks pblock_reconfigurable_logic_1] -add {RAMB36_X0Y68:RAMB36_X0Y70}
set_property SNAPPING_MODE ON [get_pblocks pblock_reconfigurable_logic_1]