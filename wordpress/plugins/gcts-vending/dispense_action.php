<?php
// This file is run when the user wishes to dispense a single item from the vending maching:

// todo: Record a log of dispense actions
// todo: Limit dispensing multiple times (i.e. look at ordered quantity, only allow that many dispense actions)

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
