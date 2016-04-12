var jQuery = jQuery.noConflict();


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
jQuery(document).ready(function() {
    jQuery(".fix-caption-code").mousedown(function() {
        jQuery("#edit-caption").val("");
    });
    jQuery(".fix-caption-code").mouseup(function() {
        quotedText = getSelectionText("#fix-caption-code");
        jQuery("#edit-caption").val(quotedText);
    });
});
