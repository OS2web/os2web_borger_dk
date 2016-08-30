<?php

/**
 * @file
 * Bartik's theme implementation to display a node.
 *
 * Available variables:
 * - $title: the (sanitized) title of the node.
 * - $content: An array of node items. Use render($content) to print them all,
 *   or print a subset such as render($content['field_example']). Use
 *   hide($content['field_example']) to temporarily suppress the printing of a
 *   given element.
 * - $user_picture: The node author's picture from user-picture.tpl.php.
 * - $date: Formatted creation date. Preprocess functions can reformat it by
 *   calling format_date() with the desired parameters on the $created variable.
 * - $name: Themed username of node author output from theme_username().
 * - $node_url: Direct URL of the current node.
 * - $display_submitted: Whether submission information should be displayed.
 * - $submitted: Submission information created from $name and $date during
 *   template_preprocess_node().
 * - $classes: String of classes that can be used to style contextually through
 *   CSS. It can be manipulated through the variable $classes_array from
 *   preprocess functions. The default values can be one or more of the
 *   following:
 *   - node: The current template type; for example, "theming hook".
 *   - node-[type]: The current node type. For example, if the node is a
 *     "Blog entry" it would result in "node-blog". Note that the machine
 *     name will often be in a short form of the human readable label.
 *   - node-teaser: Nodes in teaser form.
 *   - node-preview: Nodes in preview mode.
 *   The following are controlled through the node publishing options.
 *   - node-promoted: Nodes promoted to the front page.
 *   - node-sticky: Nodes ordered above other non-sticky nodes in teaser
 *     listings.
 *   - node-unpublished: Unpublished nodes visible only to administrators.
 * - $title_prefix (array): An array containing additional output populated by
 *   modules, intended to be displayed in front of the main title tag that
 *   appears in the template.
 * - $title_suffix (array): An array containing additional output populated by
 *   modules, intended to be displayed after the main title tag that appears in
 *   the template.
 *
 * Other variables:
 * - $node: Full node object. Contains data that may not be safe.
 * - $type: Node type; for example, story, page, blog, etc.
 * - $comment_count: Number of comments attached to the node.
 * - $uid: User ID of the node author.
 * - $created: Time the node was published formatted in Unix timestamp.
 * - $classes_array: Array of html class attribute values. It is flattened
 *   into a string within the variable $classes.
 * - $zebra: Outputs either "even" or "odd". Useful for zebra striping in
 *   teaser listings.
 * - $id: Position of the node. Increments each time it's output.
 *
 * Node status variables:
 * - $view_mode: View mode; for example, "full", "teaser".
 * - $teaser: Flag for the teaser state (shortcut for $view_mode == 'teaser').
 * - $page: Flag for the full page state.
 * - $promote: Flag for front page promotion state.
 * - $sticky: Flags for sticky post setting.
 * - $status: Flag for published status.
 * - $comment: State of comment settings for the node.
 * - $readmore: Flags true if the teaser content of the node cannot hold the
 *   main body content.
 * - $is_front: Flags true when presented in the front page.
 * - $logged_in: Flags true when the current user is a logged-in member.
 * - $is_admin: Flags true when the current user is an administrator.
 *
 * Field variables: for each field instance attached to the node a corresponding
 * variable is defined; for example, $node->body becomes $body. When needing to
 * access a field's raw values, developers/themers are strongly encouraged to
 * use these variables. Otherwise they will have to explicitly specify the
 * desired field language; for example, $node->body['en'], thus overriding any
 * language negotiation rule that was previously applied.
 *
 * @see template_preprocess()
 * @see template_preprocess_node()
 * @see template_process()
 */
?>
<div id="node-<?php print $node->nid; ?>" class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>
  <?php print render($title_prefix); ?>

<?php /**  <h2<?php print $title_attributes; ?>>
      <a href="<?php print 'node/' . $node->nid; ?>"><?php print $node->title; ?></a>
    </h2>
*/?>
  <?php print render($title_suffix); ?>


  <div id='region-content' class="content"<?php print $content_attributes; ?>>

  <?php
  if ($node->type == 'os2_borger_dk_article') {

    $content_field = array();
    $fields = $node->os2_borger_dk_article['field_settings'];
    // First get admin display settings.
    $admin_display_fields = variable_get('os2_borger_dk_display');
    $locked_os2_types = array('field_os2_borger_dk_borgerurl' => 1);
    // We get admin microarticle display settings.
    $microarticle = variable_get('os2_borger_dk_microarticle_active', FALSE);
    if ($microarticle) {
      $field_microarticle_settings = $node->os2_borger_dk_microarticle['field_microarticle_settings'];
    }

    foreach ($admin_display_fields as $type => $value) {

      // If ADMIN set this field to display.
      if ($admin_display_fields[$type]) {
        $arr = $node-> $type;

      if (count($arr) > 0 && $type != 'title' && $type != 'field_billede') {
          $content_field[$type] = $arr[LANGUAGE_NONE]['0']['value'];
        }
        elseif (count($arr) > 0 && $type == 'field_billede') {
          $filepath = $arr[LANGUAGE_NONE]['0']['uri'];
          $alt = $arr[LANGUAGE_NONE]['0']['alt'];
          $content_field[$type] = theme('image', array('path' => $filepath, 'alt' => $alt, 'title' => $alt));
        }
        else {
          $content_field[$type] = '';
       }
        // Microarticles : if microarticle is set up to show by admin.
        if ($microarticle) {
          // Check if content field is body and field_microarticle_settings
          // is NOT empty.
          // The field_microarticle_setting will be empty when a new
          // article is imported and shown in a form, then node_view
          // will display full body text.
          if ($type == 'body' && !empty($field_microarticle_settings)) {
            $body_text = $node->body[LANGUAGE_NONE]['0']['value'];

            $article_text = '';

            $doc = new DOMDocument();
            $doc->loadHTML('<?xml encoding="utf-8" ?>' . $body_text);
            $xpath = new DOMXPath($doc);

            $results = $xpath->query("//*[@class='microArticle']");

            $microno = 0;
            foreach ($results as $item) {
              foreach ($item->getElementsByTagName('h2') as $articletitle) {
                $title = trim($articletitle->nodeValue);
              }

              $text = '';
              foreach ($item->getElementsByTagName('div')->item(0)->childNodes as $articletext) {
                $text .= $doc->saveHTML($articletext);
              }
              $microno++;

              if ($field_microarticle_settings[$microno] != 0) {
                // Body text (Article text).
                $article_text .= "<div class=\"microArticle\" id=\"microArticle" . $microno . "\">" . "\r\n";

                $micro_h2 = "<h2 class=\"mArticle\" id=\"mArticle" . $microno . "\">";
                $micro_h2 .= $title . "</h2>";

                $micro_content = "<div class=\"mArticle" . $microno . " mArticle\">";
                $micro_content .= $text . "\r\n    </div>";

                $article_text .= $micro_h2 . "\r\n";
                $article_text .= $micro_content;
                $article_text .= "\r\n</div>\r\n\r\n";
              }
            }
            // Content body shows only visible microarticles/ part of body_text.
            $content_field[$type] = $article_text;
          }
        }
        elseif ($type == 'body') {
          $content_field['body'] = $node->body[LANGUAGE_NONE]['0']['value'];
        }

        // End of microarticles.
        // If EDITOR set this field to be hidden.
        if ($fields[$type] == '0') {
            $content_field[$type] = '';
        }
      }

      // If ADMIN set this field to be hidden.
      else {
          $content_field[$type] = '';
      }
    }
    drupal_add_js(drupal_get_path('module', 'os2_borger_dk') . '/js/os2_borger_dk.js', 'file');
    drupal_add_css(drupal_get_path('module', 'os2_borger_dk') . '/css/os2_borger_dk.css', 'file');

    // Set the page-title if field-value is given.
   // if (!empty($node->field_os2_borger_dk_pagetitle[LANGUAGE_NONE][0]['value'])) {
      //drupal_set_title($node->field_os2_borger_dk_pagetitle[LANGUAGE_NONE][0]['value']);
    //}
  }
  ?>

  <?php
   print "<div class='borger_dk-region-stack3'>
            <div class='inside'>";
    if (!empty($content_field['field_billede'])) {
      print "<div class='borger_dk_billede'>";
      print render($content_field['field_billede']);
      print "</div>";
    }

    if (!empty($content_field['field_os2_borger_dk_header'])) {
      print "<div class='borger_dk_header' id='borger_dk_header'>";
      print render($content_field['field_os2_borger_dk_header']);
      print "</div>";
    }
    print "</div></div>";
  ?>
  <div class="content clearfix"<?php print $content_attributes; ?>>
  <?php
    if (!empty($content_field['field_os2_borger_dk_selfservi'])) {
      print "<div class='borger_dk-region-stack2'>
              <div class='inside'>
                <div class='os2_borger_dk_selfservi'>";
      print render($content_field['field_os2_borger_dk_selfservi']);
      print   '</div>
              </div>
            </div>';
    }

  ?>
  </div>
  <div class="content clearfix"<?php print $content_attributes; ?>>
  <?php
    print "<div class='borger_dk-region-stack3'>
            <div class='inside'>";
    if (!empty($content_field['field_os2_borger_dk_pre_text'])) {
      print "<div class='borger_dk-field_os2-borger-dk-pre_text'>";
      print render($content_field['field_os2_borger_dk_pre_text']);
      print '</div>';
      print "<div class='panel-separator'></div>";
    }

    if (!empty($content_field['body'])) {
      print "<div class='borger_dk-body node-body' id='borger_dk-body'>";
      print "<div class='borger_dk_body_intro_text'>" . "Læs om " . $node->title . "</div>";
      print render($content_field['body']);
      print '</div>';
      print "<div class='panel-separator'></div>";
    }
    if (!empty($content_field['field_os2_borger_dk_post_text'])) {
      print "<div class='borger_dk-field_os2-borger-dk-post_text'>";
      print render($content_field['field_os2_borger_dk_post_text']);
      print '</div>';
      print "<div class='panel-separator'></div>";
    }

    if (!empty($content_field['field_os2_borger_dk_legislati'])) {
      print "<div class='borger_dk-field_os2-borger-dk-legislati'>";
      print render($content['field_os2_borger_dk_legislati']);
      print "</div>";
    }
    print "</div></div>";

    print "<div class='borger_dk-region-stack4'>";
    print   "<div class= 'inside'>";
    if (!empty($content_field['field_os2_borger_dk_recommend'])) { 
      print   "<div class='borger_dk-field_os2-borger-dk-recommend'>";
      print     render($content_field['field_os2_borger_dk_recommend']);
      print   "</div>";
      print   "<div class='panel-separator'></div>";

    }
    if (!empty($content_field['field_os2_borger_dk_shortlist'])) {
      print   "<div class='borger_dk-field_os2-borger-dk-shortlist'> ";
      print     render($content_field['field_os2_borger_dk_shortlist']);
      print   "</div>";
    }
    if (!empty($content_field['field_os2_borger_dk_byline'])) {
      print   "<div class='borger_dk-field_os2-borger-dk-byline'> ";
      print    render($content_field['field_os2_borger_dk_byline']);
      print   "</div>";
    }

    print "</div></div>";

//      print render($content);
    ?>
    </div>
  </div>

  <?php
    // Remove the "Add new comment" link on the teaser page or if the comment
    // form is being displayed on the same page.
   // if ($teaser || !empty($content['comments']['comment_form'])) {
      //unset($content['links']['comment']['#links']['comment-add']);
    //}
    // Only display the wrapper div if there are links.
    $links = render($content['links']);
    if ($links):
  ?>
    <div class="link-wrapper">
      <?php print $links; ?>
    </div>
  <?php endif; ?>

  <?php print render($content['comments']); ?>

</div>
