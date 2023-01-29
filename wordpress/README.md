# WordPress Vending Machine stuff

This folder contains version controlled stuff from the `wp-content/` folder in WordPress.

A description of contents are:

- `plugins/gcts-vending/` = this WordPress plugin lets us render some buttons on the page. The user can dispense products from their WooCommerce order using these buttons. It does the talking to MQTT.
- `themes/vend/` = this custom WordPress theme lets us delete a lot of the default WordPress UI and create a nice custom shop front end.
- `theme/vend/woocommerce/emails/customer-processing-order.php` = this is an "overwrite" for the default WooCommerce email that is sent our. We include a link to "dispense" items in this email.
