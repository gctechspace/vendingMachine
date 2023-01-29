<?php
/* Template Name: Vending */

// TODO: put our retro vending machine HTML/CSS/JS whatever in this page template.

get_header();

$args = array(
	'orderby'  => 'name',
);
$products = wc_get_products( $args );

$products_for_display = array();

foreach($products as $product) {
	$products_for_display[] = array(
		'name' => $product->get_name(),
		'location' => $product->get_sku(),
		'picture' => wp_get_attachment_url( $product->get_image_id() ),
	);
}

?>

<div>
	<?php foreach($products_for_display as $product) { ?>
		<div>
			name: <?php echo $product['name']; ?> <br/>
			location: <?php echo $product['location']; ?> <br/>
			picture: <?php echo $product['picture']; ?> <br/>

		</div>
	<?php } ?>
</div>

<?php get_footer(); ?>
