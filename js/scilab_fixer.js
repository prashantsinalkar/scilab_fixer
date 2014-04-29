$(document).ready(function() {
    var basePath = Drupal.settings.basePath;
    var modPath = basePath + "fix/";

    $category = $("#fix-caption-form #edit-category");
    $book = $("#fix-caption-form #edit-book");
    $chapter = $("#fix-caption-form #edit-chapter");
    $example = $("#fix-caption-form #edit-example");
    $caption = $("#fix-caption-form #edit-caption");
    $code = $("#fix-caption-form #fix-caption-code");
    $form = $("#scilab-fixer-caption-form");
    $updating = $("#fix-caption-page #updating");
    $done = $("#fix-caption-page #done");
    $example.attr("multiple", "enabled");

    function reset() {
        for (var i = 0, l = arguments.length; i < l; i ++) {
            switch(arguments[i]) {
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

    $category.change(function() {
        reset("book", "chapter", "example", "caption");
        var category_id = $(this).val();
        
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
        reset("caption");
        
        $.ajax({
            url: modPath + "ajax/example/" + example_id,
            type: "POST",
            dataType: "html",
            success: function(data) {
                var code = $(data).filter("#code").html();
                $code.html(code);
                var caption = $(data).filter("#caption").html();
                $caption.val(caption);
            }
        });
    });

    $form.submit(function(e) {
        var example_id = $example.val();
        if(example_id != "0") {
            var caption = $caption.val();
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
});
