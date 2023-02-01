<?php
// This file is run when the user wishes to dispense a single item from the vending maching:

$quantity_ordered = $valid_order_item->get_quantity();
$dispensed_count = (int)$valid_order_item->get_meta('dispensed_count');
$dispenseable = $dispensed_count < $quantity_ordered;
$valid_order_item->update_meta_data('dispensed_count', $dispensed_count + 1);
$valid_order_item->save_meta_data();

if(!$dispenseable){
	?>
	This item has already been dispensed. Thank you!
	<?php
}else{

	// Connect to our local unraid mqtt server:
	require_once 'vendor/autoload.php';
	$server   = '172.16.2.2';
	$port     = 1883;
	$clientId = 'vend';

	// Pull out the product location from WooCommerce SKU field:
	$sku = $valid_product->get_sku();

	// Send a message to our MWTT broker telling the vending machine it should dispense this product:
	$mqtt = new \PhpMqtt\Client\MqttClient($server, $port, $clientId);
	$mqtt->connect();
	$mqtt->publish('techspace/vend01/dispence', $sku, 0);
	$mqtt->disconnect();

	// Now show some nice message to the user:
	?>

	<h2>Thanks!</h2>
	<p>
		Your product has been dispensed.
	</p>

	<?php
}

// And we can now chekc if our order should be updated as complete.
$this->maybe_flag_order_as_complete($order);
