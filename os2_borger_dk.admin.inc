<?php
/**
 * @file
 * os2_borger_dk.admin.inc
 */

/**
 * Implements hook_form().
 */
function os2_borger_dk_settings_form($form, $form_state) {
  $form['vertical_settings_tab'] = array(
    '#type' => 'vertical_tabs',
    '#weight' => 0,
  );
  // Form element. It's a fieldset , "Toggle display"
  $form['os2_borger_dk_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Toggle display'),
    '#collapsible' => TRUE,
    '#group' => 'vertical_settings_tab',
  );
  // Some of the fields are locked from from being changed.
  $locked_os2_types = array(
    'field_os2_borger_dk_borgerurl' => 1,
    'field_os2_borger_dk_kle' => 2,
    'field_os2_borger_dk_formterm' => 2,
  );

  $data = field_info_instances('node', 'os2_borger_dk_article');
  $newdata = array();
  $field_name = array();
  $init = array();
  $field_name['title'] = 'title';
  $newdata['title'] = t('Title');
  $init['title'] = 'title';
  foreach ($data as $type => $item) {
    if (empty($locked_os2_types[$type])) {
      $field_name[$type] = $type;
    }
    if (empty($locked_os2_types[$type]) || (!empty($locked_os2_types[$type]) && $locked_os2_types[$type] == 1)) {
      // Only some of the locked types can be displayed.
      $newdata[$type] = $item['label'];
      $init[$type] = $type;
    }
  }
  $form_display = variable_get('os2_borger_dk_display');

  $selected = array();
  if (isset($form_display) && !empty($form_display)) {
    $selected = $form_display;
  }
  elseif (!isset($form_display)) {
    $selected = $field_name;
    variable_set('os2_borger_dk_display', $field_name);
  }
  $form['os2_borger_dk_settings']['os2_borger_dk_display'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Visible article fields'),
    '#options' => $newdata,
    '#description' => t('Check or uncheck the respective fields'),
    '#default_value' => !empty($form_display) ? $selected : $init,
    '#group' => 'vertical_settings_tab',
  );
  $form['os2_borger_dk_settings_2'] = array(
    '#type' => 'fieldset',
    '#title' => t('Editable article fields'),
    '#collapsible' => TRUE,
    '#group' => 'vertical_settings_tab',
  );
  $form_editable = variable_get('os2_borger_dk_editable');
  $selected2 = array();
  if (isset($form_editable) && !empty($form_editable)) {
    $selected2 = $form_editable;
  }
  elseif (!isset($form_editable)) {
    $selected2 = $field_name;
    variable_set('os2_borger_dk_editable', $selected2);
  }
  foreach ($locked_os2_types as $type => $locked) {
    unset($newdata[$type]);
    unset($selected2[$type]);
    unset($init[$type]);
  }
  $form['os2_borger_dk_settings_2']['os2_borger_dk_editable'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Editable article fields'),
    '#options' => $newdata,
    '#description' => t('Check or uncheck the respective fields'),
    '#default_value' => !empty($form_display) ? $selected2 : $init,
    '#group' => 'vertical_settings_tab',
  );

  $form['os2_borger_dk_settings_3'] = array(
    '#type' => 'fieldset',
    '#title' => t('Daily article syncronization'),
    '#collapsible' => TRUE,
    '#group' => 'vertical_settings_tab',
  );

  $sync_options = drupal_map_assoc(range(1, 24));
  $sync_options[0] = t('Disabled (Syncronize manually)');
  $form['os2_borger_dk_settings_3']['os2_borger_dk_nightly_article_sync'] = array(
    '#type' => 'select',
    '#title' => t('Select when imported articles should be updated'),
    '#options' => $sync_options,
    '#description' => t('At what time of day should articles be syncronized with the Borger.dk-Webservice.'),
    '#default_value' => variable_get('os2_borger_dk_nightly_article_sync'),
  );

  $form['os2_borger_dk_import_settings_4'] = array(
    '#type' => 'fieldset',
    '#title' => t('Borger.dk Webservice settings'),
    '#collapsible' => TRUE,
    '#group' => 'vertical_settings_tab',
  );

  $form['os2_borger_dk_import_settings_4']['os2_borger_dk_webservice'] = array(
    '#type' => 'textfield',
    '#title' => t('Borger.dk Webservice URL'),
    '#default_value' => variable_get('os2_borger_dk_webservice', 'https://www.borger.dk/_vti_bin/borger/ArticleExport.svc?wsdl'),
    '#group' => 'vertical_settings_tab',
    '#size' => 60,
    '#maxlength' => 130,
  );
  $form['os2_borger_dk_import_settings_4']['os2_borger_dk_webservice_request_limit'] = array(
    '#type' => 'textfield',
    '#title' => t('Webservice request limitation'),
    '#description' => t("e.g this number of items (default is 100) can be fetched within the time-period limit"),
    '#default_value' => variable_get('os2_borger_dk_webservice_request_limit', 100),
    '#group' => 'vertical_settings_tab',
    '#size' => 5,
    '#maxlength' => 10,
  );
  $form['os2_borger_dk_import_settings_4']['os2_borger_dk_webservice_time_limit'] = array(
    '#type' => 'textfield',
    '#title' => t('Webservice request time-period limitation'),
    '#description' => t("e.g within this number of seconds (default is 60) the webservice is limited to serve a given number of requests"),
    '#default_value' => variable_get('os2_borger_dk_webservice_time_limit', 60),
    '#group' => 'vertical_settings_tab',
    '#size' => 5,
    '#maxlength' => 10,
  );

  $form['os2_borger_dk_settings_5'] = array(
    '#type' => 'fieldset',
    '#title' => t('Borger.dk Title search'),
    '#collapsible' => TRUE,
    '#group' => 'vertical_settings_tab',
  );
  $form['os2_borger_dk_settings_5']['os2_borger_dk_titles_sync'] = array(
    '#type' => 'checkbox',
    '#title' => t('Import Borger.dk-articles by title instead of by URL'),
    '#description' => t('Check to enable the title-search when importing articles from Borger.dk'),
    '#default_value' => variable_get('os2_borger_dk_titles_sync', 1),
  );
  $form['os2_borger_dk_settings_5']['os2_borger_dk_titles_search_auto_submit'] = array(
    '#type' => 'checkbox',
    '#title' => t('Activate auto-submit when selecting an article to import by its title.'),
    '#description' => t('Check to enable the title-search auto-submit when importing articles from Borger.dk'),
    '#default_value' => variable_get('os2_borger_dk_titles_search_auto_submit', 1),
  );

  $form['os2_borger_dk_settings_6'] = array(
    '#type' => 'fieldset',
    '#title' => t('Borger.dk Menu import'),
    '#collapsible' => TRUE,
    '#group' => 'vertical_settings_tab',
  );
  $form['os2_borger_dk_settings_6']['os2_borger_dk_menus_import'] = array(
    '#type' => 'checkbox',
    '#title' => t('Import and utilize Borger.dk-menus'),
    '#description' => t('Check to enable the menu-import from Borger.dk.'),
    '#default_value' => variable_get('os2_borger_dk_menus_import', 0),
  );
  $menu_page = node_type_get_types();
  foreach ($menu_page as $key => $object) {
    $menu_page[$key] = $object->name;
  }

  $form['os2_borger_dk_settings_6']['os2_borger_dk_menus_menu_page'] = array(
    '#type' => 'select',
    '#options' => $menu_page,
    '#title' => t('Import Menu page to content type'),
    '#description' => t('Select menu page content type'),
    '#default_value' => variable_get('os2_borger_dk_menus_menu_page', 'os2_base_contentpage'),
  );

  $term_vocabularies = taxonomy_get_vocabularies();
  $term_options = array();
  if ($term_vocabularies) {
    foreach ($term_vocabularies as $term_parent) {
      $term_name = $term_parent->name;
      $term = $term_parent->machine_name;
      $term_options[$term] = $term_name;
    }
  }
  $form['os2_borger_dk_settings_6']['os2_borger_dk_choose_term'] = array(
    '#type' => 'checkbox',
    '#title' => t('Import menu link to term'),
    '#description' => t('Import menu link to term, not content type.'),
    '#default_value' => variable_get('os2_borger_dk_choose_term', 0),
  );
  $form['os2_borger_dk_settings_6']['build_borger_menu_under_vocabulary'] = array(
    '#type' => 'select',
    '#title' => t('Select Borger.dk menu Parent Taxonomy Item'),
    '#options' => $term_options,
    '#description' => t('Select Borger.dk menu Parent Item from the list. REMEMBER to save before you start Menu import.'),
    '#default_value' => variable_get('build_borger_menu_under_vocabulary', 'os2_base_tax_site_structure'),
    '#type' => 'select',
  );
  $form['os2_borger_dk_settings_6']['os2_borger_dk_borger_dk_menu_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Menu name for imported Borger.dk-menu'),
    '#description' => t("The imported menu will be put into a local menu by the name given here."),
    '#default_value' => variable_get('os2_borger_dk_borger_dk_menu_name', 'Borger.dk menu'),
  );
  //$sync_options = drupal_map_assoc(range(1, 24));
  //$sync_options[0] = t('Disabled (No menu syncronization)');
  /*$form['os2_borger_dk_settings_6']['os2_borger_dk_nightly_menu_sync'] = array(
    '#type' => 'select',
    '#title' => t('Select when the menu-structure should be imported'),
    '#options' => $sync_options,
    '#description' => t('At what time of day should menus be syncronized with the Borger.dk-Webservice.'),
    '#default_value' => variable_get('os2_borger_dk_nightly_menu_sync'),
  );
  $form['os2_borger_dk_settings_6']['os2_borger_dk_weekly_borger_dk_menus_import'] = array(
    '#type' => 'checkbox',
    '#title' => t('Should Borger.dk-menus be updated weekly'),
    '#description' => t('Check to enable weekly menu-import from Borger.dk (otherwise menus are imported daily)'),
    '#default_value' => variable_get('os2_borger_dk_weekly_borger_dk_menus_import', 0),
  );*/
  $form['os2_borger_dk_settings_6']['os2_borger_dk_menu_import_description'] = array(
    '#type' => 'fieldset',
    '#title' => t('Description for importing Borger.dk menu structure'),
    '#description' => t("Before you import Borger.dk menu structure, please choose a content type
                        which default is OS2web base contentpage <b><u>(Indholdsside)</u></b>.
                        Step1 : Please import Borger.dk-menu items now.
                        Step2 : When Borger.dk menu items are imported, please import all Borger.dk articles.
                        Step 3 : Create Borger.dk menu structure now."),
  );
  $menu_option = array();
  $existing_menu = db_query('SELECT * FROM {menu_custom}');
  $menu_option['borgerdkmenu2'] = variable_get('os2_borger_dk_borger_dk_menu_name', 'Borger.dk menu');
  if ($existing_menu) {
    foreach ($existing_menu as $menu_item) {
      $menu_name = $menu_item->menu_name;
      $menu_title = $menu_item->title;
      $menu_option[$menu_name] = $menu_title;
    }
  }
  $form['os2_borger_dk_settings_6']['build_borger_menu_under_menu'] = array(
    '#type' => 'select',
    '#title' => t('Select Borger.dk menu Parent Item'),
    '#options' => $menu_option,
    '#description' => t('Select Borger.dk menu Parent Item from the list. REMEMBER to save before you start Menu import.'),
    '#default_value' => variable_get('build_borger_menu_under_menu', 'borgerdkmenu2'),
    '#type' => 'select',
  );
  // variable_set('build_borger_menu_under_menu');

  $disabled = FALSE;
  $description = '';
  $menus_import = variable_get('os2_borger_dk_menus_import', 0);
  if ($menus_import == 0) {
    $disabled = TRUE;
    $description = t('If you want to import the Borger.dk menu you must first activate "Import and utilize Borger.dk-menus" and save the settings');
    drupal_set_message($description);
  }
  $form['os2_borger_dk_settings_6']['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Import Borger.dk-menu now',
    '#submit' => array('os2_borger_dk_import_menu_now_form_submit'),
    '#disabled' => $disabled,

   // '#description' => t(' Step1 : Please import Borger.dk menu items.'),
  );
  $form['os2_borger_dk_settings_6']['import_all_articles_now'] = array(
    '#type' => 'submit',
    '#value' => 'Import Borger.dk articles',
    '#submit' => array('os2_borger_dk_import_articles_now_form_submit'),
    '#disabled' => $disabled,
    '#description' => t('Step2 : When Borger.dk menu items are imported, please import all Borger.dk articles.'),
  );
  $form['os2_borger_dk_settings_6']['create_borger_dk_menu_links'] = array(
    '#type' => 'submit',
    '#value' => t('Create Borger.dk menu structure now'),
    '#submit' => array('os2_borger_dk_create_borger_dk_menu_now_form_submit'),
    '#disabled' => $disabled,
    '#description' => t('Step 3 : Create Borger.dk menu structure now.'),
  );

  $form['os2_borger_dk_settings_7'] = array(
    '#type' => 'fieldset',
    '#title' => t('Microarticles visibility and editability'),
    '#collapsible' => TRUE,
    '#group' => 'vertical_settings_tab',
  );
  $form['os2_borger_dk_settings_7']['os2_borger_dk_microarticle_active'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show and edit microarticles individually'),
    '#description' => t('Check to enable visibility and editablity of microarticles from Borger.dk'),
    '#default_value' => variable_get('os2_borger_dk_microarticle_active', 0),
  );

  $options = variable_get('os2_borger_dk_municipality_list');
  if (isset($options)) {
    $muni_items = $options;
  }
  else {
    $muni_items = array();
    $municipalities = _os2_borger_dk_GetMunicipalityList();
    $muni_items[0] = t('--Not selected--');
    foreach ($municipalities as $code => $item) {
      $muni_items[$item['MunicipalityCode']] = $item['MunicipalityName'];
    }
    variable_set('os2_borger_dk_municipality_list', $muni_items);
  }

  $admin_municipality = variable_get('os2_borger_dk_municipality_active');
  $form['os2_borger_dk_settings_8'] = array(
    '#type' => 'fieldset',
    '#title' => t('Municipality import option'),
    '#collapsible' => TRUE,
    '#group' => 'vertical_settings_tab',
  );
  $form['os2_borger_dk_settings_8']['os2_borger_dk_municipality_active'] = array(
    '#type' => 'select',
    '#title' => t('Select municipality'),

    '#options' => $muni_items,
    '#description' => t('Check to import articles from Borger.dk according to municipality.'),
    '#default_value' => $admin_municipality,
  );
  $form['os2_borger_dk_settings_9'] = array(
    '#type' => 'fieldset',
    '#title' => t('Import duplicate'),
    '#collapsible' => TRUE,
    '#group' => 'vertical_settings_tab',
  );
  $form['os2_borger_dk_settings_9']['os2_borger_dk_import_duplicate'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow article import duplicated.'),
    '#description' => t('Allow to import aricle many times.'),
    '#default_value' => variable_get('os2_borger_dk_import_duplicate', 0),
  );
  return system_settings_form($form);
}

/**
 * Function _os2_borger_dk_GetMunicipalityList().
 *
 * This function uses the Borger.dk Webservice GetMunicipalityList
 * in order to fetch a list of Municipality-codes from Borger.dk
 */
function _os2_borger_dk_GetMunicipalityList($wsdl = 'https://www.borger.dk/_vti_bin/borger/ArticleExport.svc?wsdl') {
  $errors = array();
  $municipality_item = array();

  // Then we can start using a new soap-client to
  // find the article-ID from the Borger.dk-URL.
  $client = new SoapClient($wsdl, array('exceptions' => 0));
  // First we make sure to keep requests/minute within given limits.
  // _os2_borger_dk_articles_check_webservice_constraints();
  // Then we execute our webservice-request.
  $result_list = $client->GetMunicipalityList();
  if (is_soap_fault($result_list)) {
    $errors = _os2_borger_dk_articles_translate_soap_fault('GetMunicipalityList', $result_list->faultcode, $result_list->faultstring);
    return $errors;
  }

  $municipality_item = array();
  // Then we get the result-list from the specified webservice.
  $municipality_list = $result_list->GetMunicipalityListResult;
  if (is_soap_fault($municipality_list)) {
    $errors = _os2_borger_dk_articles_translate_soap_fault('GetMunicipalityListResult', $municipality_list->faultcode, $municipality_list->faultstring);

    return $errors;
  }
  else {
    // Now we step through all the fetched menu-items and convert them to
    // arrays of wanted field-sets.
    if (is_array($municipality_list->Municipality)) {
      foreach ($municipality_list->Municipality as $id => $menu_item) {
        $new_item = _os2_borger_dk_articles_parse_borgerdk_municipality_item($menu_item);
        $municipality_item[] = $new_item;
      }
    }
    else {
      $new_item = _os2_borger_dk_articles_parse_borgerdk_municipality_item($municipality_list->Municipality);
      $municipality_item[] = $new_item;
    }
  }

  return $municipality_item;
}

/**
 * Function _os2_borger_dk_articles_parse_borgerdk_municipality_item().
 *
 * This function parses a Borger.dk-ArticleDescription and extracts all
 * the field values we want for our list of titles (for autocompletion etc)
 */
function _os2_borger_dk_articles_parse_borgerdk_municipality_item($menu_item) {
  $borgerdk_menu = array(
    'MunicipalityCode'    => $menu_item->MunicipalityCode,
    'MunicipalityName' => $menu_item->MunicipalityName,
  );

  return $borgerdk_menu;
}
