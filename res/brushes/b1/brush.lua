dst_pos_x = 0.0
dst_pos_y = 0.0
dst_angle = 0.0
dst_scale_x = 1.0
dst_scale_y = 1.0
dst_opacity = 1.0

function f(src_pos_x, src_pos_y, src_pressure, src_dt, src_norm_x, src_norm_y, src_tan_x, src_tan_y, src_tilt_x, src_tilt_y)
    return src_pos_x, src_pos_y, 0.0, 1.0, 1.0, 1.0
end

