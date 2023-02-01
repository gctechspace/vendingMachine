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
	$quantity_ordered = $order_item->get_quantity();

//	$order_item->update_meta_data('dispensed_count', 0);
//	$order_item->save_meta_data();

	$dispensed_count = (int)$order_item->get_meta('dispensed_count');
	$dispenseable = $dispensed_count < $quantity_ordered;
	?>
	<div style="padding: 10px; margin: 10px; border: 1px solid #ccc">
		<strong>
			<?php echo $quantity_ordered;?> x <?php echo $product->get_name();?>
		</strong>
		 in location <?php echo $product->get_sku(); ?>. <br/>
		<?php if($dispenseable){ ?>
			<a href="<?php echo $this->get_dispense_url($order_id, $item_id);?>" target="_blank">Click here to dispense this item</a>
		<?php }else{ ?>
			(Thank you, this item has already been dispensed)
		<?php } ?>
	</div>
	<?php
}
?>
