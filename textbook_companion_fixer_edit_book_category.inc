<?php
function textbook_companion_fixer_edit_book_proposal_all()
{
	//get the book count
	
	$result = db_query("SELECT COUNT( pe.book ) AS book_count FROM textbook_companion_preference pe LEFT JOIN textbook_companion_proposal po ON pe.proposal_id = po.id WHERE po.proposal_status =3 AND pe.approval_status =1 AND pe.category>0");
		$row = $result->fetchObject();
		$book_count = $row->book_count;
	$i=1;
	
		/* get preference */
		$preference_q = db_query("SELECT pe.id as pref_id, pe.book as book, pe.author as author,pe.category as existing_category, pe.publisher as publisher, pe.year as year, pe.id as pe_id, po.approval_date as approval_date
  FROM {textbook_companion_preference} pe LEFT JOIN {textbook_companion_proposal} po ON pe.proposal_id = po.id
  WHERE po.proposal_status = 3 AND pe.approval_status = 1 ORDER BY pe.book ASC");   
		while ($preference_data = $preference_q->fetchObject())
	{
		$proposal_rows[] = array(
			$i,
			"{$preference_data->book} <br> <em>by {$preference_data->author}</em>",
			_textbook_companion_fixer_list_of_category($preference_data->existing_category),
			'',
			'',
			l('Edit', 'textbook_companion_fixer/category_edit/' . $preference_data->pref_id) 
		);
		$i++;
	} //$proposal_data = $proposal_q->fetchObject()
	/* check if there are any pending proposals */
	if (!$proposal_rows)
	{
		drupal_set_message(t('There are no proposals.'), 'status');
		return '';
	} //!$proposal_rows
	$output .= "Book count with category: " . $book_count;
	$proposal_header = array(
		'No.',
		'Title of the Book',
		'Existing Category',
		'New Category',
		'Sub Category',
		'Action'
	);
	$output .= theme('table', array(
		'header' => $proposal_header,
		'rows' => $proposal_rows
	));
	return $output;
}
/******************************************************************************/
/**************************** CATEGORY EDIT FORM ******************************/
/******************************************************************************/
function textbook_companion_fixer_category_edit_form($form, &$form_state)
{
	/* get current proposal */
	$preference_id = arg(2);
	$query = db_select('textbook_companion_preference');
	$query->fields('textbook_companion_preference');
	$query->condition('id', $preference_id);
	$preference_q = $query->execute();
	$preference_data = $preference_q->fetchObject();
	if (!$preference_data)
	{
		drupal_set_message(t('Invalid book selected. Please try again.'), 'error');
		drupal_goto('manage_proposal/category');
		return;
	} //!$preference_data
	$form["wrapper"] = array(
		"#type" => "fieldset",
		"#title" => "Edit the existing book category to new category",
		"#prefix" => "<div id='fix-tbc-category-form'>",
		"#suffix" => "</div>"
	);
	$form["wrapper"]['book'] = array(
		'#type' => 'item',
		'#title' => t('Title of the book'),
		'#markup' => $preference_data->book
	);
	$form["wrapper"]['author'] = array(
		'#type' => 'item',
		'#title' => t('Author Name'),
		'#markup' => $preference_data->author
	);
	$form["wrapper"]['isbn'] = array(
		'#type' => 'item',
		'#title' => t('ISBN No'),
		'#markup' => $preference_data->isbn
	);
	$form["wrapper"]['publisher'] = array(
		'#type' => 'item',
		'#title' => t('Publisher & Place'),
		'#markup' => $preference_data->publisher
	);
	$form["wrapper"]['edition'] = array(
		'#type' => 'item',
		'#title' => t('Edition'),
		'#markup' => $preference_data->edition
	);
	$form["wrapper"]['year'] = array(
		'#type' => 'item',
		'#title' => t('Year of pulication'),
		'#markup' => $preference_data->year
	);
		$form["wrapper"]['main_category']= array(
			"#markup" => _textbook_companion_fixer_list_of_category_checkboxes(),
		);
	$form["wrapper"]['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit'),
		'#attributes' => array('id' => 'submit-button-category'),
		"#prefix" => "<div class='update-button-category'>",
		"#suffix" => "</div>"
	);
	$form["wrapper"]['cancel'] = array(
		'#markup' => l(t('Cancel'), 'textbook_companion_fixer/edit_book_category')
	);
	return $form;
}
