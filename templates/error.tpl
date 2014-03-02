<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html>
<head>
 <title>MQWeb - ERROR</title>
 <link href="/static/css/style.css" rel="stylesheet" type="text/css"/>
 <script src="//ajax.googleapis.com/ajax/libs/jquery/1.10.2/jquery.min.js"></script>
 <script src="/static/js/center.js" type="text/javascript"> </script>
 <script type="text/javascript">
  jQuery(document).ready(function()
                         {
                           jQuery("#content").center();
                         });
 </script>
</head>
<body>
 <div style="padding:10px;border:3px solid red;background-color:#FF6666" id="content">
   <img style="float:left;display:block;" src="/static/images/error.png" alt="error" />
   <div style="float:left;padding-left:10px;">
    Object: <?= error.object ?><br />
    Function: <?= error.fn ?><br />
    Code: <?= error.code ?><br />
    Reason: <?= error.reason.code ?> - <?= error.reason.desc ?>
   </div>
 </div>
</body>
</html>
