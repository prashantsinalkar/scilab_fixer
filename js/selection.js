function getSelectionText(divID) {
	var selectedText = "";
	if (window.getSelection) {
		var sel = window.getSelection();
		var div = document.getElementById(divID);
		if (sel.rangeCount) {
			// Get the selected range
			var range = sel.getRangeAt(0);
			// Check that the selection is wholly contained within the div text
			// if (range.commonAncestorContainer == div.firstChild) {
			var selectedText = range.toString();
			// }
		}
	}
	return selectedText;
}


(function($) {
	$(document).ready(function() {
		$(".fix-caption-code").mousedown(function() {
			$("#edit-caption").val("");
		});
		$(".fix-caption-code").mouseup(function() {
			quotedText = getSelectionText("#fix-caption-code");
			$("#edit-caption").val(quotedText);
		});
		$("#edit-example").mousedown(function() {
			$("#edit-caption").val("");
		});
		$("#edit-example").mouseup(function() {
			quotedText =  $('option:selected', this).attr("data-exampleid");
			$("#edit-caption").val(quotedText);
		});
	});
})(jQuery);
