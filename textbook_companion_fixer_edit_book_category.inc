<?php
/*
function textbook_companion_fixer_edit_book_category(){

}
*/
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
/*
function _edit_category_all(){
	// get pending proposals to be approved 
	$preference_rows = array();
	//$preference_q = db_query("SELECT * FROM {textbook_companion_preference} WHERE approval_status = 1 ORDER BY id DESC");
	$query = db_select('textbook_companion_preference');
	$query->fields('textbook_companion_preference');
	$query->condition('approval_status', 1);
	$query->orderBy('id', 'DESC');
	$preference_q = $query->execute();
	while ($preference_data = $preference_q->fetchObject())
	{
		switch ($preference_data->category)
		{
			case 0:
				$category_data = 'Not Selected';
				break;
			case 1:
				$category_data = 'Fluid Mechanics';
				break;
			case 2:
				$category_data = 'Control Theory & Control Systems';
				break;
			case 3:
				$category_data = 'Chemical Engineering';
				break;
			case 4:
				$category_data = 'Thermodynamics';
				break;
			case 5:
				$category_data = 'Mechanical Engineering';
				break;
			case 6:
				$category_data = 'Signal Processing';
				break;
			case 7:
				$category_data = 'Digital Communications';
				break;
			case 8:
				$category_data = 'Electrical Technology';
				break;
			case 9:
				$category_data = 'Mathematics & Pure Science';
				break;
			case 10:
				$category_data = 'Analog Electronics';
				break;
			case 11:
				$category_data = 'Digital Electronics';
				break;
			case 12:
				$category_data = 'Computer Programming';
				break;
			case 13:
				$category_data = 'Others';
				break;
			default:
				$category_data = 'Unknown';
				break;
		} //$preference_data->category
		$preference_rows[] = array(
			$preference_data->book . "<br> <i>by " . $preference_data->author . "</i>",
			$preference_data->isbn,
			$preference_data->publisher,
			$preference_data->edition,
			$preference_data->year,
			$category_data,
			l('Edit', 'manage_proposal/category/edit/' . $preference_data->id)
		);
	} //$preference_data = $preference_q->fetchObject()
	$preference_header = array(
		'Book',
		'ISBN',
		'Publisher',
		'Edition',
		'Year',
		'Category',
		'Status'
	);
	$output = theme('table', array(
		'header' => $preference_header,
		'rows' => $preference_rows
	));
	return $output;

}
*/
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
	$form['book'] = array(
		'#type' => 'item',
		'#title' => t('Title of the book'),
		'#markup' => $preference_data->book
	);
	$form['author'] = array(
		'#type' => 'item',
		'#title' => t('Author Name'),
		'#markup' => $preference_data->author
	);
	$form['isbn'] = array(
		'#type' => 'item',
		'#title' => t('ISBN No'),
		'#markup' => $preference_data->isbn
	);
	$form['publisher'] = array(
		'#type' => 'item',
		'#title' => t('Publisher & Place'),
		'#markup' => $preference_data->publisher
	);
	$form['edition'] = array(
		'#type' => 'item',
		'#title' => t('Edition'),
		'#markup' => $preference_data->edition
	);
	$form['year'] = array(
		'#type' => 'item',
		'#title' => t('Year of pulication'),
		'#markup' => $preference_data->year
	);


	//main_category = _textbook_companion_fixer_list_of_category_checkboxes();
		$query = db_select('list_of_category');
		$query->fields('list_of_category');
		//$query->fields(array('category_id','main_category'));
		$query->orderBy('category_id', 'ASC');
		$category_list = $query->execute();
		
   
	while ($category_list_data = $category_list->fetchObject())
	{
		$categoryname = $category_list_data->main_category;
		if($categoryname!=null||strlen($categoryname)!=0){
		$category[$category_list_data->category_id] = $category_list_data->main_category;

		$form['main_category'.$category_list_data->category_id]= array(
			'#type' => 'checkbox',
			'#title' => $category_list_data->main_category,	
			'#ajax' => array(
				'wrapper' => 'ajax-subcategory-list-replace-'. $category_list_data->category_id,
				'callback' => 'ajax_subcategory_list_callback_'.$category_list_data->category_id
			),
		);
		$form['main_subcategory_'. $category_list_data->category_id]= array(
			'#type' => 'select',
			'#options' => $category_list_data->main_category,
			'#prefix' => '<div id="ajax-subcategory-list-replace-'. $category_list_data->category_id.'">',
			'#suffix' => '</div>',
		);

		} //$category_list_data = $category_list->fetchObject()
	}
	
	/* orm['main_category'] = array(
		'#type' => 'checkboxes',
		'#title' => t('Main category'),
		'#options' => $main_category,
		'#required' => TRUE
	);*/

	$form['category'] = array(
		'#type' => 'select',
		'#title' => t('Category'),
		'#options' => _tbc_list_of_category(),
		'#required' => TRUE,
		'#default_value' => $preference_data->category
	);
	$form['submit'] = array(
		'#type' => 'submit',
		'#value' => t('Submit')
	);
	$form['cancel'] = array(
		'#type' => 'markup',
		'#value' => l(t('Cancel'), 'textbook_companion_fixer/edit_book_category')
	);
	return $form;
}
function textbook_companion_fixer_category_edit_form_submit($form, &$form_state)
{
	// get current proposal 
	$preference_id = (int) arg(3);
	//preference_q = db_query("SELECT * FROM {textbook_companion_preference} WHERE id = %d", $preference_id);
	$preference_data = db_fetch_object($preference_q);
	$query = db_select('textbook_companion_preference');
	$query->fields('textbook_companion_preference');
	$query->condition('id', $preference_id);
	$preference_q = $query->execute();
	$preference_data = $preference_q->fetchObject();
	if (!$preference_data)
	{
		drupal_set_message(t('Invalid book selected. Please try again.'), 'error');
		drupal_goto('textbook_companion_fixer/edit_book_category');
		return;
	} //!$preference_data
	//db_query("UPDATE {textbook_companion_preference} SET category = %d WHERE id = %d", $form_state['values']['category'], $preference_data->id);
	$query = db_update('textbook_companion_preference');
	$query->fields(array(
		'category' => $form_state['values']['category']
	));
	$query->condition('id', $preference_data->id);
	$num_updated = $query->execute();
	drupal_set_message(t('Book Category Updated'), 'status');
	drupal_goto('textbook_companion_fixer/edit_book_category');
}


/********************* Ajax callback ***************************/
function ajax_subcategory_list_callback_1($form, $form_state)
{
	$category_default_value = 1;
	if ($category_default_value > 0)
	{
		$form['main_subcategory_1']['#options'] = _textbook_companion_fixer_list_of_subcategory($category_default_value);
		$commands[] = ajax_command_replace("#ajax-subcategory-list-replace-1", drupal_render($form['main_subcategory_1']));	

	} //$category_default_value > 0
	else
	{
		$form['main_subcategory_1']['#options'] = _textbook_companion_fixer_list_of_subcategory();
		$commands[] = ajax_command_replace("#ajax-subcategory-list-replace-1", drupal_render($form['main_subcategory_1']));
		
	}
	return array(
		'#type' => 'ajax',
		'#commands' => $commands
	);
}
/*************************************************************************/
