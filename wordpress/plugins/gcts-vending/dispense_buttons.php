<?php
// This file is displayed when the user clicks the "dispense things" link in the order confirmation email.
// We show a list of ordered products with individual "dispense me" links next to each one:
?>

<h2>Thank you for your order</h2>

<p>When you're ready for the items to be dispensed from the vending machine please click the buttons below:</p>

<?php
foreach($order_items as $order_item){
	$item_id = $order_item->get_id();
	$product      = $order_item->get_product();
	?>
	<div style="padding: 10px; margin: 10px; border: 1px solid #ccc">
		Item: <?php echo $product->get_name();?> in location <?php echo $product->get_sku(); ?>. <br/>
		<a href="<?php echo $this->get_dispense_url($order_id, $item_id);?>" target="_blank">Dispense me</a>
	</div>
	<?php
}
?>
