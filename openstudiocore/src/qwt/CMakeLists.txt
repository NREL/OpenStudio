set(target_name zkqwt)

if(UNIX)
  string(REGEX REPLACE "\\-Werror" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
  string(REGEX REPLACE "\\-Wall" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
  string(REGEX REPLACE "\\-Weffc\\+\\+" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")
elseif(CMAKE_CL_64)
  # Disable warnings as errors for qwt since we are trying to not modify it too much
  string(REGEX REPLACE "\\/WX" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
endif()

# source files
set(${target_name}_src
  qwt.h
  qwt_abstract_scale.cpp
  qwt_abstract_scale.h
  qwt_abstract_scale_draw.cpp
  qwt_abstract_scale_draw.h
  qwt_abstract_slider.cpp
  qwt_abstract_slider.h
  qwt_analog_clock.cpp
  qwt_analog_clock.h
  qwt_array.h
  qwt_arrow_button.cpp
  qwt_arrow_button.h
  qwt_clipper.cpp
  qwt_clipper.h
  qwt_color_map.cpp
  qwt_color_map.h
  qwt_compass.cpp
  qwt_compass.h
  qwt_compass_rose.cpp
  qwt_compass_rose.h
  qwt_counter.cpp
  qwt_counter.h
  qwt_curve_fitter.cpp
  qwt_curve_fitter.h
  qwt_data.cpp
  qwt_data.h
  qwt_dial.cpp
  qwt_dial.h
  qwt_dial_needle.cpp
  qwt_dial_needle.h
  qwt_double_interval.cpp
  qwt_double_interval.h
  qwt_double_range.cpp
  qwt_double_range.h
  qwt_double_rect.cpp
  qwt_double_rect.h
  qwt_dyngrid_layout.cpp
  qwt_dyngrid_layout.h
  qwt_event_pattern.cpp
  qwt_event_pattern.h
  qwt_global.h
  qwt_interval_data.cpp
  qwt_interval_data.h
  qwt_knob.cpp
  qwt_knob.h
  qwt_layout_metrics.cpp
  qwt_layout_metrics.h
  qwt_legend.cpp
  qwt_legend.h
  qwt_legend_item.cpp
  qwt_legend_item.h
  qwt_legend_itemmanager.h
  qwt_magnifier.cpp
  qwt_magnifier.h
  qwt_math.cpp
  qwt_math.h
  qwt_painter.cpp
  qwt_painter.h
  qwt_paint_buffer.cpp
  qwt_paint_buffer.h
  qwt_panner.cpp
  qwt_panner.h
  qwt_picker.cpp
  qwt_picker.h
  qwt_picker_machine.cpp
  qwt_picker_machine.h
  qwt_plot.cpp
  qwt_plot.h
  qwt_plot_axis.cpp
  qwt_plot_canvas.cpp
  qwt_plot_canvas.h
  qwt_plot_curve.cpp
  qwt_plot_curve.h
  qwt_plot_dict.cpp
  qwt_plot_dict.h
  qwt_plot_grid.cpp
  qwt_plot_grid.h
  qwt_plot_item.cpp
  qwt_plot_item.h
  qwt_plot_layout.cpp
  qwt_plot_layout.h
  qwt_plot_magnifier.cpp
  qwt_plot_magnifier.h
  qwt_plot_marker.cpp
  qwt_plot_marker.h
  qwt_plot_panner.cpp
  qwt_plot_panner.h
  qwt_plot_picker.cpp
  qwt_plot_picker.h
  qwt_plot_print.cpp
  qwt_plot_printfilter.cpp
  qwt_plot_printfilter.h
  qwt_plot_rasteritem.cpp
  qwt_plot_rasteritem.h
  qwt_plot_rescaler.cpp
  qwt_plot_rescaler.h
  qwt_plot_scaleitem.cpp
  qwt_plot_scaleitem.h
  qwt_plot_spectrogram.cpp
  qwt_plot_spectrogram.h
  # qwt_plot_svgitem.cpp # DLM: I don't think our build of QT includes this
  qwt_plot_svgitem.h
  qwt_plot_xml.cpp
  qwt_plot_zoomer.cpp
  qwt_plot_zoomer.h
  qwt_polygon.h
  qwt_raster_data.cpp
  qwt_raster_data.h
  qwt_round_scale_draw.cpp
  qwt_round_scale_draw.h
  qwt_scale_div.cpp
  qwt_scale_div.h
  qwt_scale_draw.cpp
  qwt_scale_draw.h
  qwt_scale_engine.cpp
  qwt_scale_engine.h
  qwt_scale_map.cpp
  qwt_scale_map.h
  qwt_scale_widget.cpp
  qwt_scale_widget.h
  qwt_slider.cpp
  qwt_slider.h
  qwt_spline.cpp
  qwt_spline.h
  qwt_symbol.cpp
  qwt_symbol.h
  qwt_text.cpp
  qwt_text.h
  qwt_text_engine.cpp
  qwt_text_engine.h
  qwt_text_label.cpp
  qwt_text_label.h
  qwt_thermo.cpp
  qwt_thermo.h
  qwt_valuelist.h
  qwt_wheel.cpp
  qwt_wheel.h
)

# moc files
set(${target_name}_moc
  qwt_abstract_slider.h
  qwt_analog_clock.h
  qwt_compass.h
  qwt_counter.h
  qwt_dial.h
  qwt_dyngrid_layout.h
  qwt_knob.h
  qwt_legend.h
  qwt_legend_item.h
  qwt_magnifier.h
  qwt_panner.h
  qwt_picker.h
  qwt_plot.h
  qwt_plot_canvas.h
  qwt_plot_magnifier.h
  qwt_plot_panner.h
  qwt_plot_picker.h
  qwt_plot_zoomer.h
  qwt_scale_widget.h
  qwt_slider.h
  qwt_text_label.h
  qwt_thermo.h
  qwt_wheel.h
)

qt5_wrap_cpp_minimally(${target_name}_moc_src ${${target_name}_moc})

add_definitions(-DQWT_DLL)
add_library(${target_name} SHARED
  ${${target_name}_src}
  ${${target_name}_moc_src}
)

target_link_libraries(${target_name}
  ${QT_LIBS}
)

if(NOT APPLE)
  install(TARGETS ${target_name}
    RUNTIME DESTINATION bin 
    LIBRARY DESTINATION lib
  )
endif()
