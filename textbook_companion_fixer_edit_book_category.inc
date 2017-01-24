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
			l('Edit', 'textbook_companion_fixer/category_edit' . $proposal_data->id) 
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
