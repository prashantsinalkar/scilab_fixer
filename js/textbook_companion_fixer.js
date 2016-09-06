(function($) {
	$(document).ready(function() {
		var basePath = Drupal.settings.basePath;
		var modPath = basePath + "textbook_companion_fixer/";
		var modPath1 = basePath + "textbook_companion_fixer/aicte/book/";
		$category = $("#fix-tbc-form #edit-category");
		$book = $("#fix-tbc-form #edit-book");
		$chapter = $("#fix-tbc-form #edit-chapter");
		$example = $("#fix-tbc-form #edit-example");
		$caption = $("#fix-tbc-form #edit-caption");
		$code = $("#fix-tbc-form #edit-code");
		$caption_form = $("#scilab-fixer-caption-form");
		$code_form = $("#scilab-fixer-code-form");
		$updating = $("#fix-tbc-page #updating");
		$done = $("#fix-tbc-page #done");
		$example.attr("multiple", "enabled");

		function reset() {
			for (var i = 0, l = arguments.length; i < l; i++) {
				switch (arguments[i]) {
					case "book":
						$book.html("<option value='0'>Please select a book</option>");
						break;
					case "chapter":
						$chapter.html("<option value='0'>Please select a chapter</option>");
						break;
					case "example":
						$example.html("<option value='0'>Please select a example</option>");
						break;
					case "caption":
						$caption.val("");
						break;
				}
			}
		}
		$(".select-book").hide();
		$(".select-chapter").hide();
		$(".select-example").hide();
		$(".enter-caption").hide();
		$(".example-code-edit").hide();
		$(".well").hide();
		$(".update-button").hide();
		$category.change(function() {
			reset("book", "chapter", "example", "caption");
			var category_id = $(this).val();
			if (category_id < 1) {
				$(".select-book").hide();
				$(".select-chapter").hide();
				$(".select-example").hide();
				$(".enter-caption").hide();
				$(".example-code-edit").hide();
				(".well").hide();
				$(".update-button").hide();
			} else {
				$(".select-book").show();
				$(".select-chapter").hide();
				$(".select-example").hide();
				$(".enter-caption").hide();
				$(".example-code-edit").hide();
				$(".well").hide();
				$(".update-button").hide();
			}
			$.ajax({
				url: modPath + "ajax/category/" + category_id,
				type: "POST",
				dataType: "html",
				success: function(data) {
					$book.html(data);
				}
			});
		});
		$book.change(function() {
			reset("chapter", "example", "caption");
			var book_id = $(this).val();
			if (book_id < 1) {
				$(".select-chapter").hide();
				$(".select-example").hide();
				$(".enter-caption").hide();
				$(".example-code-edit").hide();
				$(".well").hide();
				$(".update-button").hide();
			} else {
				$(".select-chapter").show();
				$(".select-example").hide();
				$(".enter-caption").hide();
				$(".example-code-edit").hide();
				$(".well").hide();
				$(".update-button").hide();
			}
			$.ajax({
				url: modPath + "ajax/book/" + book_id,
				type: "POST",
				dataType: "html",
				success: function(data) {
					$chapter.html(data);
				}
			});
		});
		$chapter.change(function() {
			reset("example", "caption");
			var chapter_id = $(this).val();
			if (chapter_id < 1) {
				$(".select-example").hide();
				$(".enter-caption").hide();
				$(".example-code-edit").hide();
				$(".well").hide();
				$(".update-button").hide();
			} else {
				$(".select-example").show();
				$(".enter-caption").hide();
				$(".example-code-edit").hide();
				$(".update-button").hide();
			}
			$.ajax({
				url: modPath + "ajax/chapter/" + chapter_id,
				type: "POST",
				dataType: "html",
				success: function(data) {
					$example.html(data);
				}
			});
		});
		$example.change(function() {
			var example_id = $(this).val();
			var example_caption = $(this).text();
			reset("caption");
			if (example_id < 1) {
				$(".enter-caption").hide();
				// $("#edit-caption").val("");
				$(".example-code-edit").hide();
				$(".well").hide();
				$(".update-button").hide();
			} else {
				$(".enter-caption").show();
				//  $("#edit-caption").val(example_caption);
				$(".example-code-edit").show();
				$(".well").show();
				$(".update-button").show();
			}
			$.ajax({
				url: modPath + "ajax/example/" + example_id,
				type: "POST",
				dataType: "html",
				success: function(data) {
					var code = $(data).filter("#code").html();
					/* checking whether it is for .well or textarea */
					if ($code.hasClass("fix-caption-code")) {
						$code.html(code);
					} else {
						$code.val(code);
					}
					var caption = $(data).filter("#caption").html();
					try {
						$caption.val(caption);
					} catch (e) {
						return;
					}
				}
			});
		});
		$caption_form.submit(function(e) {
			var example_id = $example.val();
			if (example_id != "0") {
				var caption = $caption.val();
				caption = caption.trim();
				if (caption == '') {
					alert('Please enter new caption ');
					return false;
				}
				$updating.show();
				$.ajax({
					url: modPath + "ajax/update/",
					type: "POST",
					data: {
						example_id: example_id,
						caption: caption
					},
					dataType: "html",
					success: function(data) {
						$chapter.trigger("change");
						$updating.hide();
						$done.show();
						$done.fadeOut("slow");
					}
				});
			} else {
				alert("No example selected.")
			}
			e.preventDefault();
		});
		$code_form.submit(function(e) {
			var example_id = $example.val();
			if (example_id != "0") {
				var code = $code.val();
				code = code.trim();
				if (code == '') {
					alert('Please enter new code');
					return false;
				}
				$.ajax({
					url: modPath + "ajax/code/" + example_id,
					type: "POST",
					data: {
						code: code
					},
					dataType: "html",
					success: function(data) {
						$chapter.trigger("change");
						$updating.hide();
						$done.show();
						$done.fadeOut("slow");
						$(".example-code-edit").show();
					}
				});
			} else {
				alert("No example selected.")
			}
			e.preventDefault();
		});
		$Selected = $(".selected");
		$Selected.click(function(e) {
			$(".sync-msg").remove();
			$(this).after("<span class='sync-msg'>Saving...</span>");
			$.ajax({
				url: modPath1 + "ajax/selected/" + $(this).attr("data-bid"),
				success: function() {
					$(".sync-msg").remove();
					console.log("success");
				}
			});
		});
		/* toggle in edition */
		$ind_ed = $(".ind-ed");
		$ind_ed.click(function(e) {
			var aicte_id = $(this).attr("data-aicte");
			$t = $(this);
			$.ajax({
				url: modPath + "ajax/ind-ed/" + aicte_id,
				type: "GET",
				dataType: "html",
				success: function(data) {
					$tr = $t.parents("tr:first");
					if ($tr.hasClass("orange")) {
						$t.parents("tr:first").removeClass("orange");
						$t.html("Mark");
					} else {
						$t.parents("tr:first").addClass("orange");
						$t.html("Unmark");
					}
				},
			});
			e.preventDefault();
		});
	});
})(jQuery);
