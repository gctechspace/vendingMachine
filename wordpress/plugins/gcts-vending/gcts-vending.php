<?php
/**
 * Plugin Name: GCTS Vending WooCommerce Tweaks
 * Description: Some custom WooCommerce tweaks for vending machine
 * Plugin URI: https://vend.gctechspace.org
 * Version: 1.0.0
 * Author: dtbaker
 * Author URI: http://dtbaker.net
 * Text Domain: gcts-vending
 */


class GctsVending {
	public function init() {
		// This is the shortcode we will put on the /dispense/ page, allowing us to render dispense buttons and stuff from the email link.
		add_shortcode( 'gcts_vending_dispense_buttons', array( $this, 'gcts_vending_dispense_buttons' ) );
	}

	/**
	 * This is the callback function from the above shortcode.
	 * We get some query params from the URL, verify the hash, pull out some order details, and then render some files.
	 *
	 * @return null
	 */
	public function gcts_vending_dispense_buttons(){
		ob_start();

		$order_id = !empty($_GET['order_id']) ? (int)$_GET['order_id'] : false;
		$order_hash = !empty($_GET['hash']) ? $_GET['hash'] : false;
		$item_id = !empty($_GET['item_id']) ? (int)$_GET['item_id'] : false;

		if(!$order_id || !$order_hash || $order_hash !== $this->secure_order_hash($order_id) ){
			echo 'Invalid Order Link, Please contact GC TeshSpace';
		} else {
			$order = new WC_Order( $order_id );
			$order_data = $order->get_data();
			$order_items = $order->get_items();
			if($item_id){
				$valid_item_id = false;
				$valid_product = false;
				foreach($order_items as $order_item){
					$this_item_id = $order_item->get_id();
					if($this_item_id == $item_id){
						$valid_item_id = $this_item_id;
						$valid_product      = $order_item->get_product();
					}
				}
				if(!$valid_item_id || !$valid_product){
					echo 'Invalid Item ID, Please contact GC TeshSpace';
				}else {
					include 'dispense_action.php';
				}
			}else{
				include 'dispense_buttons.php';
			}
		}

		return ob_get_clean();
	}

	/**
	 * Generate our secure URL for our order confirmation email.
	 * Clicking this link will show the user their purchased products and allow them to be dispensed.
	 *
	 * @param $order_id - the WooCommerce order ID
	 *
	 * @return string
	 */
	public function collect_url($order_id) {
		return "https://vend.gctechspace.org/dispense/?order_id=" . $order_id .'&hash=' . $this->secure_order_hash($order_id);
	}

	/**
	 * This link is the secure link that will let the user dispense an individual item from an order
	 *
	 * @param $order_id - the WooCommerce order ID
	 * @param $item_id - the WooCommerce item ID from the above order
	 *
	 * @return string
	 */
	public function get_dispense_url($order_id, $item_id) {
		return "https://vend.gctechspace.org/dispense/?order_id=" . $order_id .'&hash=' . $this->secure_order_hash($order_id) ."&item_id=" . $item_id;
	}

	/**
	 * A little helper to generate a secure hash used in the link emails.
	 * Nothing too fancy with timeouts and stuff, just a md5 with our secret NONCE_SALT from local wp-config.php file
	 *
	 * @param $order_id - the WooCommerce order ID
	 *
	 * @return string
	 */
	private function secure_order_hash($order_id){
		return md5('vending order ' . $order_id . ' super secret ' . NONCE_SALT);
	}


	/**
	 * Just some stuff that lets us do "GctsVending::get_instance()" from anywhere in WordPress:
	 */
	private static $instance = null;
	public static function get_instance() {
		if ( ! self::$instance ) {
			self::$instance = new self;
		}
		return self::$instance;
	}
}

// Start things up and register our hooks or shortcodes:
GctsVending::get_instance()->init();
