
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<HTML>
<HEAD>
  <TITLE>Graphics formats supported by IVCon</TITLE>
  <META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">

<?php
   require_once '../include/basics.php';
   $home_path="../";
   $file_ext="ASC";
   addGlobalHeaderTags();
?>

</HEAD>

<?php
   require_once $home_path.'include/sidebar.php';
?>

<BODY TOPMARGIN=0 LEFTMARGIN=0 MARGINWIDTH=0 MARGINHEIGHT=0>

<div class="big_hdr" id="titlebar">Supported 3D formats</div>

<div id="toplogo"><img <?php echo "src=\"".$home_path."/pictures/ivcon_icon_64.png\""; ?>
 ALT="IVCon logo" TITLE="IVCon logo"/></div>

<?php drawSidebar('sidebar_formt','format_asc'); ?>

<div id="page_content">

<h3><?php echo $file_ext; ?> format overview:</h3>
<p>
  ASC is a format of points cloud. It defines only point coordinates,
   in a way that makes them easy to read in any program. It does not have
   any header or additional information.
</p>
<hr>

<h3><?php echo $file_ext; ?> file characteristics:</h3>
<p>
  <ul>
  <li>ASCII text</li>
  <li>Bare 3D vertices data</li>
  <li>No faces or meshes definition</li>
  <li>No lines definition</li>
  <li>No cameras or lights definition</li>
  <li>No color or textured</li>
  <li>No compression</li>
  </ul>
</p>
<hr>

<h3><?php echo $file_ext; ?> format references and information:</h3>
<p>
  this is a specific IVCon format, and it does not have any known references
</p>
<hr>

<h3><?php echo $file_ext; ?> support notes inside IVCon source code:</h3>
<p>
<?php printCodeComment($home_path."/src/asc.cpp",256); ?>
</p>

<hr>
<?php addGlobalFooterTags(); ?>

</div>

<BR>

</BODY>
</HTML>
 