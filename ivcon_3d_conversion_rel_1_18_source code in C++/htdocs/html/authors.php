
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<HTML>
<HEAD>
  <TITLE>IVCon authors</TITLE>
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

<div class="big_hdr" id="titlebar">The IVCon authors</div>

<div id="toplogo"><img <?php echo "src=\"".$home_path."/pictures/ivcon_icon_64.png\""; ?>
 ALT="IVCon logo" TITLE="IVCon logo"/></div>

<?php drawSidebar('sidebar_authrs'); ?>

<div id="page_content">


<p>
Core author is <b>John Burkardt</b>. Helpful corrections to the 3DS reader
 were supplied by <b>John F Flanagan</b>.
</p>
<p>
The GMOD reader and writer were written and supplied by <b>Zik Saleeba</b>.<br>
Improvements to the DXF routines to handle polygon oriented files were
 supplied by <b>Zik Saleeba</b>.
</p>
<p>
The 3DS reader is based on a program by <b>Martin van Velsen</b>, with the help
 of <b>Gert van der Spoel</b>.
</p>
<p>
Divided into small files, corrections and improvements, normals computation
 by <b>Philippe Guglielmetti</b>.
</p>
<p>
Rewritten, vierified, corrected many bugs, improved 3DS filter and main
 class by <b>Tomasz Lis</b>.
</p>

<p>
<b>You may be one of the authors too.</b> Just download the source code and add
 new function or fix some bugs. Then, contact one of the project admins
 to become a developer, or just send your modifications to an admin and
 he will do the rest.
</p>

<hr>

<?php addGlobalFooterTags(); ?>

<p>
Note that this program is public domain and open source. Because of that,
 there is no single owner of this program. Every person that feels involved
 with this tool may claim to be one of the owners.
</p>

</div>

<BR>

</BODY>
</HTML>
 