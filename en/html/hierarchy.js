var hierarchy =
[
    [ "dxle::color::color_base", "a00002.html", [
      [ "dxle::color::dx_color", "a00009.html", null ],
      [ "dxle::color::rgb", "a00041.html", null ]
    ] ],
    [ "conditional", null, [
      [ "dxle::type_traits::detail::float_promote1< T >", "a00017.html", [
        [ "dxle::type_traits::detail::float_promote_impl< T >", "a00021.html", null ]
      ] ]
    ] ],
    [ "dxle::impl::Counter_template< T >", "a00004.html", null ],
    [ "dxle::impl::Counter_template< U >", "a00004.html", [
      [ "dxle::impl::Counter_template< T >::Cast< U >", "a00001.html", null ]
    ] ],
    [ "dxle::detail::distance_result_type_impl< T, is_integral, is_float >", "a00006.html", null ],
    [ "dxle::detail::distance_result_type_impl< decltype(std::declval< std::remove_cv_t< T1 >>()-std::declval< std::remove_cv_t< T2 >>())>", "a00006.html", [
      [ "dxle::distance_result_type< T1, T2 >", "a00005.html", null ]
    ] ],
    [ "dxle::detail::distance_result_type_impl< T, false, true >", "a00007.html", null ],
    [ "dxle::detail::distance_result_type_impl< T, true, false >", "a00008.html", null ],
    [ "dxle::color::dx_color_param", "a00010.html", null ],
    [ "false_type", null, [
      [ "dxle::type_traits::has_operator_notequal_to_this_impl< T, typename >", "a00024.html", null ],
      [ "dxle::type_traits::has_operator_notequal_to_zero_impl< T, typename >", "a00026.html", null ],
      [ "dxle::type_traits::has_operator_notequal_to_this_impl< T >", "a00024.html", [
        [ "dxle::type_traits::has_operator_notequal_to_this< T >", "a00023.html", null ]
      ] ],
      [ "dxle::type_traits::has_operator_notequal_to_zero_impl< T >", "a00026.html", [
        [ "dxle::type_traits::has_operator_notequal_to_zero< T >", "a00025.html", null ]
      ] ]
    ] ],
    [ "dxle::type_traits::first_enabled< Args >", "a00011.html", null ],
    [ "dxle::type_traits::first_enabled< Args... >", "a00011.html", [
      [ "dxle::type_traits::first_enabled< std::enable_if< false, T >, Args... >", "a00012.html", null ]
    ] ],
    [ "dxle::type_traits::first_enabled< enable_if< 1!=sizeof(T), T >, enable_if< std::is_signed< T >::value, int >, unsigned int >", "a00011.html", [
      [ "dxle::detail::use_big_type_when_one_byte_impl< T, true >", "a00054.html", null ]
    ] ],
    [ "dxle::type_traits::first_enabled< std::enable_if< true, T >, Args... >", "a00013.html", null ],
    [ "dxle::type_traits::first_enabled< T, Args... >", "a00014.html", null ],
    [ "dxle::type_traits::first_type< T,... >", "a00015.html", null ],
    [ "dxle::type_traits::detail::float_promote2< T, U, is_arithmetic >", "a00018.html", null ],
    [ "dxle::type_traits::detail::float_promote2< T, U, true >", "a00019.html", null ],
    [ "dxle::type_traits::detail::float_promote_impl< Types >", "a00020.html", null ],
    [ "dxle::type_traits::detail::float_promote_impl< dxle::type_traits::detail::float_promote2< T, U >::type, Tail... >", "a00020.html", [
      [ "dxle::type_traits::detail::float_promote_impl< T, U, Tail... >", "a00022.html", null ]
    ] ],
    [ "dxle::type_traits::detail::float_promote_impl< std::remove_cv< Types >::type... >", "a00020.html", [
      [ "dxle::type_traits::float_promote< Types >", "a00016.html", null ]
    ] ],
    [ "is_base_of", null, [
      [ "dxle::color::is_colortype< T >", "a00030.html", null ]
    ] ],
    [ "is_same", null, [
      [ "dxle::type_traits::is_representable< from, to >", "a00032.html", null ]
    ] ],
    [ "iterator", null, [
      [ "dxle::utility::counter_iterator< T, >", "a00003.html", null ]
    ] ],
    [ "dxle::utility::nth_loop< T, >", "a00033.html", null ],
    [ "dxle::detail::operator_bool_helper_impl< T, has_operator_bool, has_operator_notequal_to_zero, is_compareable_with_default_ctor >", "a00035.html", null ],
    [ "dxle::detail::operator_bool_helper_impl< T, false, false, true >", "a00036.html", null ],
    [ "dxle::detail::operator_bool_helper_impl< T, false, true, is_compareable_with_default_ctor >", "a00037.html", null ],
    [ "dxle::detail::operator_bool_helper_impl< T, true, has_operator_notequal_to_zero, is_compareable_with_default_ctor >", "a00038.html", null ],
    [ "dxle::point3d_c< T, >", "a00039.html", null ],
    [ "dxle::point_c< T, >", "a00040.html", null ],
    [ "dxle::graph2d::screen_mutex_c", "a00043.html", null ],
    [ "dxle::size_c< T, >", "a00044.html", null ],
    [ "dxle::detail::static_cast_if_helper< from, to, do_cast, is_nothrow_move_constructible >", "a00046.html", null ],
    [ "dxle::detail::static_cast_if_helper< to, from, false, false >", "a00047.html", null ],
    [ "dxle::detail::static_cast_if_helper< to, from, false, true >", "a00048.html", null ],
    [ "true_type", null, [
      [ "dxle::type_traits::has_operator_notequal_to_zero_impl< T, enable_if_t< ignore_type< decltype(std::declval< T >()!=0)>::value, nullptr_t >", "a00027.html", null ],
      [ "dxle::type_traits::ignore_type< typename >", "a00028.html", null ],
      [ "dxle::type_traits::nullptr_t< T >", "a00034.html", null ]
    ] ],
    [ "type", null, [
      [ "dxle::type_traits::is_castable< From, To >", "a00029.html", null ],
      [ "dxle::type_traits::is_nothrow_convertable< From, To >", "a00031.html", null ]
    ] ],
    [ "dxle::impl::Unique_Handle_Bace< Child >", "a00050.html", null ],
    [ "dxle::impl::Unique_HandledObject_Bace< Child >", "a00051.html", null ],
    [ "dxle::impl::Unique_HandledObject_Bace< sound >", "a00051.html", [
      [ "dxle::sound::sound", "a00045.html", null ]
    ] ],
    [ "dxle::impl::Unique_HandledObject_Bace< texture_2d >", "a00051.html", [
      [ "dxle::graph2d::texture_2d", "a00049.html", [
        [ "dxle::graph2d::screen", "a00042.html", null ]
      ] ]
    ] ],
    [ "dxle::detail::use_big_type_when_one_byte_impl< T, is_arithmetic >", "a00053.html", null ],
    [ "dxle::detail::use_big_type_when_one_byte_impl< T, std::is_arithmetic< T >::value >", "a00053.html", [
      [ "dxle::use_big_type_when_one_byte< T >", "a00052.html", null ]
    ] ]
];