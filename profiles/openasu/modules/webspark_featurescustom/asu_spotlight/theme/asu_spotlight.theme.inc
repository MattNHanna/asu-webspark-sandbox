<?php
/**
 * @file
 * Theming functions for the ASU Spotlight module.
 *
 *
 *
 */

/**
 * Default theme implementation for flexslider_list
 */
function theme_asu_spotlight_list(&$vars) {
  // Reference configuration variables

  $optionset = &$vars['settings']['optionset'];
  $items = &$vars['items'];
  $attributes = &$vars['settings']['attributes'];
  $type = &$vars['settings']['type'];
  $output = '';
  // Build the list
  if (!empty($items)) {
    $output .= "<$type" . drupal_attributes($attributes) . '>';
    foreach ($items as $i => $item) {

      $caption = '';
      if (!empty($item['caption'])) {
        $caption = $item['caption'];
      }

      $output .= theme('asu_spotlight_list_item', array(
        'package' => $item,
        'settings' => array(
          'optionset' => $optionset,
        ),
        'caption' => $caption,
      ));
    }
    $output .= "</$type>";
  }

  return $output;
}

/**
 * Default theme implementation for flexslider_list_item
 */
function theme_asu_spotlight_list_item(&$vars) {
  $vars['package']['item']['overlaytextwidth'] = (isset($vars['package']['item']['overlaytextwidth'])) ? $vars['package']['item']['overlaytextwidth'] : 'full-width';
  $list_item = '<li' . drupal_attributes($vars['settings']['attributes']) . '>
            <div class="slidecontainer" position="relative">' .
    $vars['package']['slide'] .
    '<div class="blurbcontainer"> ' .
    '<div class="blurbshell ' . $vars['package']['item']['position'] . ' ' .
    $vars['package']['item']['overlaytextwidth'] . '">' .
    '<a href="' . $vars['package']['item']['link'] . '"><h1>' . $vars['package']['item']['title'] . '</h1></a>' .
    '<p>' . $vars['package']['item']['description'] . '</p>';

  if ($vars['package']['item']['color'] == 'red') {
    $color = 'primary';
  }
  else {
    $color = 'blue';
  }

  if ($vars['package']['item']['actiontitle'] != NULL && $vars['package']['item']['actionlink'] != NULL) {

    $list_item .= '<a class="btn btn-' . $color . ' btn-block" href="' . $vars['package']['item']['actionlink'] . '">'
      . $vars['package']['item']['actiontitle'] . '</a>';
  }

  if ($vars['package']['item']['actiontitle2'] != NULL && $vars['package']['item']['actionlink2'] != NULL) {

    $list_item .= '<a class="btn btn-' . $color . ' btn-block" href="' . $vars['package']['item']['actionlink2'] . '">'
      . $vars['package']['item']['actiontitle2'] . '</a>';
  }

  $list_item .= '</div>
              </div>
            </div>
          </li>';

  return $list_item;

}

/**
 * Template preprocess handler for 'asu_spotlight' theme.
 */
function template_process_asu_spotlight(&$vars) {
  // Reference configuration variables

  $optionset = &$vars['settings']['optionset'];
  $settings = &$vars['settings'];
  $items = &$vars['items'];

  // Set the default container type
  if (empty($settings['type'])) {
    $settings['type'] = 'ul';
  }

  // Load the selected optionset
  if (!empty($optionset)) {
    $optionset = flexslider_optionset_load($optionset);
  }

  // Check if an optionset was loaded
  if (empty($optionset)) {
    // Fall back to 'default' option set
    $optionset = flexslider_optionset_load('default');
    watchdog('asu_spotlight', 'Fallback to default optionset.', array(), WATCHDOG_WARNING);
  }

  // Configure attributes for containing elements
  $attributes = array();
  // Merge with defined attributes
  if (isset($settings['attributes']) and is_array($settings['attributes'])) {
    $attributes += $settings['attributes'];
  }

  // Set the ID for each flexslider instance if none is provided
  if (empty($attributes['id'])) {
    $flexslider_id = &drupal_static('flexslider_id', 0);
    $attributes['id'] = 'flexslider-' . ++$flexslider_id;
  }

  // Add the namespace to any classes
  // @todo figure out what this is supposed to do
  if (!empty($attributes['class']) && !empty($optionset->options['namespace'])) {
    foreach ($attributes['class'] as $key => $value) {
      $attributes['class'][$key] = $optionset->options['namespace'] . $value;
    }
  }

  // Add the flexslider class to be namespaced
  $attributes['class'][] = 'flexslider';

  // Add the attributes to the settings array.
  $settings['attributes'] = $attributes;

  // Finally, add the configuration to the page
  flexslider_add($vars['settings']['attributes']['id'], $vars['settings']['optionset']);

}

/**
 * Process function for asu_spotlight_list_item
 */
function template_process_asu_spotlight_list(&$vars) {

  // Reset the list of attributes
  $vars['settings']['attributes'] = array(
    // @todo find a way to detect the outter container class if possible
    'class' => array('slides'),
  );

}

/**
 * Process function for asu_spotlight_list_item
 */
function template_process_asu_spotlight_list_item(&$vars) {
  // Reset the list of attributes

  $vars['settings']['attributes'] = array();

  // Reference configuration variables
  $item = &$vars['item'];
  $settings = &$vars['settings'];
  $caption = &$vars['caption'];
  $attributes = &$vars['settings']['attributes'];

  // Generated thumbnail support
  if (isset($settings['optionset']->options['controlNav']) and $settings['optionset']->options['controlNav'] === "thumbnails") {
    // If the thumbnails are enabled in the option set, scan for the first img
    // tag and extract the src attribute to set as the thumbnail data
    $src = array();
    preg_match("<img.+?src=[\"'](.+?)[\"'].+?>", $item, $src);

    if (!empty($src[1])) {
      $attributes['data-thumb'] = $src[1];
    }
  }

  if (isset($settings['optionset']->options['thumbCaptions']) and $settings['optionset']->options['thumbCaptions'] and !empty($caption)) {
    $attributes['data-thumbcaption'] = $caption;
    // Prevent captions from appearing in the slider as well
    if (isset($settings['optionset']->options['thumbCaptionsBoth']) and FALSE === $settings['optionset']->options['thumbCaptionsBoth']) {
      $caption = '';
    }
  }

  if (!empty($caption)) {
    $caption = '<div class="flex-caption">' . $caption . '</div>';
  }
}
