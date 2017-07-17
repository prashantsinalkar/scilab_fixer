<?php
/**
 * Implementation of hook_mail().
 */
function textbook_companion_fixer_mail($key, &$message, $params)
{
	global $user;
	$language = $message['language'];
	switch ($key) {
		case 'chapter_example_updated':
			// bcc to textbook_companion_emails
			//$message['headers'] += $tbc_bcc_emails;
			/*$example_q = db_query("SELECT * FROM {textbook_companion_example} WHERE id = %d LIMIT 1", $params['example_updated']['example_id']);
			$example_data = db_fetch_object($example_q);*/
			$query = db_select('textbook_companion_example');
			$query->fields('textbook_companion_example');
			$query->condition('id', $params['chapter_example_updated']['example_id']);
			$query->range(0, 1);
			$result = $query->execute();
			$example_data = $result->fetchObject();
			$query = db_select('textbook_companion_chapter');
			$query->fields('textbook_companion_chapter');
			$query->condition('id', $example_data->chapter_id);
			$query->range(0, 1);
			$result = $query->execute();
			$chapter_data = $result->fetchObject();
			$query = db_select('textbook_companion_preference');
			$query->fields('textbook_companion_preference');
			$query->condition('id', $chapter_data->preference_id);
			$query->range(0, 1);
			$result = $query->execute();
			$preference_data = $result->fetchObject();
			$user_data = user_load($params['chapter_example_updated']['user_id']);
			$message['headers'] = $params['chapter_example_updated']['headers'];
			$message['subject'] = t('[!site_name][Textbook companion] You have updated chapter, example caption for textbook ' . $preference_data->book, array(
				'!site_name' => variable_get('site_name', '')
			), array(
				'language' => $language->language
			));
			$message['body'] = array(
				'body' => t('
Dear !user_name,

You have updated the following example:

Title of the book : ' . $preference_data->book . '
Title of the chapter : ' . $chapter_data->name . '
Example number : ' . $example_data->number . '
Caption : ' . $example_data->caption . '


Best Wishes,

Scilab TBC Team,
FOSSEE, IIT Bombay', array(
					'!site_name' => variable_get('site_name', ''),
					'!user_name' => $user_data->name
				), array(
					'language' => $language->language
				))
			);
			break;
		case 'chapter_updated':
			$query = db_select('textbook_companion_chapter');
			$query->fields('textbook_companion_chapter');
			$query->condition('id', $params['chapter_updated']['chapter_id']);;
			$query->range(0, 1);
			$result = $query->execute();
			$chapter_data = $result->fetchObject();
			$query = db_select('textbook_companion_preference');
			$query->fields('textbook_companion_preference');
			$query->condition('id', $chapter_data->preference_id);
			$query->range(0, 1);
			$result = $query->execute();
			$preference_data = $result->fetchObject();
			$user_data = user_load($params['chapter_updated']['user_id']);
			$message['headers'] = $params['chapter_updated']['headers'];
			$message['subject'] = t('[!site_name][Textbook companion] You have updated chapter name for textbook ' . $preference_data->book, array(
				'!site_name' => variable_get('site_name', '')
			), array(
				'language' => $language->language
			));
			$message['body'] = array(
				'body' => t('
Dear !user_name,

You have updated the following example:

Title of the book : ' . $preference_data->book . '
Title of the chapter : ' . $chapter_data->name . '

Best Wishes,

Scilab TBC Team,
FOSSEE, IIT Bombay', array(
					'!site_name' => variable_get('site_name', ''),
					'!user_name' => $user_data->name
				), array(
					'language' => $language->language
				))
			);
			break;
		case 'example_updated':
			$query = db_select('textbook_companion_example');
			$query->fields('textbook_companion_example');
			$query->condition('id', $params['example_updated']['example_id']);
			$query->range(0, 1);
			$result = $query->execute();
			$example_data = $result->fetchObject();
			$query = db_select('textbook_companion_chapter');
			$query->fields('textbook_companion_chapter');
			$query->condition('id', $example_data->chapter_id);
			$query->range(0, 1);
			$result = $query->execute();
			$chapter_data = $result->fetchObject();
			$query = db_select('textbook_companion_preference');
			$query->fields('textbook_companion_preference');
			$query->condition('id', $chapter_data->preference_id);
			$query->range(0, 1);
			$result = $query->execute();
			$preference_data = $result->fetchObject();
			$user_data = user_load($params['example_updated']['user_id']);
			$message['headers'] = $params['example_updated']['headers'];
			$message['subject'] = t('[!site_name][Textbook companion] You have updated example caption for textbook ' . $preference_data->book, array(
				'!site_name' => variable_get('site_name', '')
			), array(
				'language' => $language->language
			));
			$message['body'] = array(
				'body' => t('
Dear !user_name,

You have updated the following example:

Title of the book : ' . $preference_data->book . '
Title of the chapter : ' . $chapter_data->name . '
Example number : ' . $example_data->number . '
Caption : ' . $example_data->caption . '


Best Wishes,

Scilab TBC Team,
FOSSEE, IIT Bombay', array(
					'!site_name' => variable_get('site_name', ''),
					'!user_name' => $user_data->name
				), array(
					'language' => $language->language
				))
			);
			break;
			case 'new_category_updated':
			$query = db_select('textbook_companion_preference');
			$query->fields('textbook_companion_preference');
			$query->condition('id', $params['category_updated']['pref_id']);
			$query->range(0, 1);
			$result = $query->execute();
			$preference_data = $result->fetchObject();
			$query_mcat = db_select('list_of_category');
			$query_mcat->fields('list_of_category');
			$query_mcat->condition('category_id', $params['category_updated']['main_category']);
			$query_mcat->range(0, 1);
			$result_mcat = $query_mcat->execute();
			$main_category_data = $result_mcat->fetchObject();
			$query_subcat = db_select('list_of_subcategory');
			$query_subcat->fields('list_of_subcategory');
			$query_subcat->condition('subcategory_id', $params['category_updated']['sub_category']);
			$query_subcat->range(0, 1);
			$result_subcat = $query_subcat->execute();
			$sub_category_data = $result_subcat->fetchObject();
			$user_data = user_load($params['category_updated']['user_id']);
			$message['headers'] = $params['category_updated']['headers'];
			$message['subject'] = t('[!site_name][Textbook companion][category] You have updated category for textbook ' . $preference_data->book, array(
				'!site_name' => variable_get('site_name', '')
			), array(
				'language' => $language->language
			));
			$message['body'] = array(
				'body' => t('
Dear !user_name,

You have updated the category for following book :

Title of the book : ' . $preference_data->book . '
Author : ' . $preference_data->author. '

Main category : '. $main_category_data->maincategory .'
Sub category : '. $sub_category_data->subcategory .'

Best Wishes,

Scilab TBC Team,
FOSSEE, IIT Bombay', array(
					'!site_name' => variable_get('site_name', ''),
					'!user_name' => $user_data->name
				), array(
					'language' => $language->language
				))
			);
			break;
			case 'new_subcategory_deleted':
			$query = db_select('textbook_companion_preference');
			$query->fields('textbook_companion_preference');
			$query->condition('id', $params['subcategory_deleted']['pref_id']);
			$query->range(0, 1);
			$result = $query->execute();
			$preference_data = $result->fetchObject();
			$query_mcat = db_select('list_of_category');
			$query_mcat->fields('list_of_category');
			$query_mcat->condition('category_id', $params['subcategory_deleted']['main_category']);
			$query_mcat->range(0, 1);
			$result_mcat = $query_mcat->execute();
			$main_category_data = $result_mcat->fetchObject();
			$query_subcat = db_select('list_of_subcategory');
			$query_subcat->fields('list_of_subcategory');
			$query_subcat->condition('subcategory_id', $params['subcategory_deleted']['sub_category']);
			$query_subcat->range(0, 1);
			$result_subcat = $query_subcat->execute();
			$sub_category_data = $result_subcat->fetchObject();
			$user_data = user_load($params['subcategory_deleted']['user_id']);
			$message['headers'] = $params['subcategory_deleted']['headers'];
			$message['subject'] = t('[!site_name][Textbook companion][category] You have deleted subcategory for textbook ' . $preference_data->book, array(
				'!site_name' => variable_get('site_name', '')
			), array(
				'language' => $language->language
			));
			$message['body'] = array(
				'body' => t('
Dear !user_name,

You have deleted the subcategory for following book :

Title of the book : ' . $preference_data->book . '
Author : ' . $preference_data->author. '

Main category : '. $main_category_data->maincategory .'
Sub category : '. $sub_category_data->subcategory .'

Best Wishes,

Scilab TBC Team,
FOSSEE, IIT Bombay', array(
					'!site_name' => variable_get('site_name', ''),
					'!user_name' => $user_data->name
				), array(
					'language' => $language->language
				))
			);
			break;
			case 'new_maincategory_deleted':
			$query = db_select('textbook_companion_preference');
			$query->fields('textbook_companion_preference');
			$query->condition('id', $params['maincategory_deleted']['pref_id']);
			$query->range(0, 1);
			$result = $query->execute();
			$preference_data = $result->fetchObject();
			$query_mcat = db_select('list_of_category');
			$query_mcat->fields('list_of_category');
			$query_mcat->condition('category_id', $params['maincategory_deleted']['main_category']);
			$query_mcat->range(0, 1);
			$result_mcat = $query_mcat->execute();
			$main_category_data = $result_mcat->fetchObject();
			$user_data = user_load($params['maincategory_deleted']['user_id']);
			$message['headers'] = $params['maincategory_deleted']['headers'];
			$message['subject'] = t('[!site_name][Textbook companion][category] You have deleted main category for textbook ' . $preference_data->book, array(
				'!site_name' => variable_get('site_name', '')
			), array(
				'language' => $language->language
			));
			$message['body'] = array(
				'body' => t('
Dear !user_name,

You have deleted the main category with subcategory for following book :

Title of the book : ' . $preference_data->book . '
Author : ' . $preference_data->author. '

Main category : '. $main_category_data->maincategory .'

Best Wishes,

Scilab TBC Team,
FOSSEE, IIT Bombay', array(
					'!site_name' => variable_get('site_name', ''),
					'!user_name' => $user_data->name
				), array(
					'language' => $language->language
				))
			);
			break;
	} //$key
}
