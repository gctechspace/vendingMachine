<?php

function vend_scripts() {
	wp_enqueue_style('vend-style', get_stylesheet_uri(), array(), filemtime('/var/www/html/wp-content/themes/vend/style.css'));
}

add_action('wp_enqueue_scripts', 'vend_scripts');
