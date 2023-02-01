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
		'sku' => $product->get_sku(),
		'image' => wp_get_attachment_url( $product->get_image_id() ),
		'url' => get_permalink($product->get_id()),
		'stock' => $product->get_stock_quantity()
	);
}

?>
<script>
	var vendingproducts = <?= json_encode($products_for_display); ?>;

</script>


<div class="bg">
    <h1>TECHSPACE VENDING</h1>
    <!-- <video autoPlay muted loop id="bgVideo" class="video-background">
        <source src="media/video.webm" type="video/mp4" />
    </video> -->
    <div class="inner-vend">
        <div class="grid-container grid-container--5" id="js-grid-container-one">
        </div>
        <div class="grid-container grid-container--10" id="js-grid-container-two">
        </div>
        <div class="grid-container grid-container--5" id="js-grid-container-three">
        </div>
    </div>


    <script src="/wp-content/themes/vend/index.js?v=<?php echo filemtime('/var/www/html/wp-content/themes/vend/index.js');?>"></script>
</div>

<?php get_footer(); ?>
