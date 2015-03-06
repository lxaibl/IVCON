
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<HTML>
<HEAD>
  <TITLE>How to use IVCon</TITLE>
  <META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">

<?php
   require_once '../include/basics.php';
   $home_path="../";
   addGlobalHeaderTags();
?>

</HEAD>

<?php
   require_once $home_path.'include/sidebar.php';
?>

<BODY TOPMARGIN=0 LEFTMARGIN=0 MARGINWIDTH=0 MARGINHEIGHT=0>

<div class="big_hdr" id="titlebar">Using IVCon</div>

<div id="toplogo"><img <?php echo "src=\"".$home_path."/pictures/ivcon_icon_64.png\""; ?>
 ALT="IVCon logo" TITLE="IVCon logo"/></div>

<?php drawSidebar('sidebar_usage'); ?>

<div id="page_content">


<p>
  The program can be used in command line form, as in:
  </p><blockquote>
<b>ivcon</b> <i>input.iv</i> <i>output.stl</i>
  </blockquote>
  or can be used in an interactive mode, in which input and output
  files can be read or converted in any order.
<p></p>

<p>
  Graphics file formats are poorly documented; hence this program
  was written primarily by examining typical files and struggling
  to interpret them.  Particularly in the case of Inventor and DXF
  files, the program is not able to handle all the subtleties of 
  the file format, and may mangle data, or even crash.  
</p>

<hr>
<?php addGlobalFooterTags(); ?>

</div>

<BR>

</BODY>
</HTML>
 