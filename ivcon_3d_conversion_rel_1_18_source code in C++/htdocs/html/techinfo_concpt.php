
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<HTML>
<HEAD>
  <TITLE>IVCon technical information</TITLE>
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

<div class="big_hdr" id="titlebar">Technical information</div>

<div id="toplogo"><img <?php echo "src=\"".$home_path."/pictures/ivcon_icon_64.png\""; ?>
 ALT="IVCon logo" TITLE="IVCon logo"/></div>

<?php drawSidebar('sidebar_techinfo','techinfo_concpt'); ?>

<div id="page_content">

<h3 align="center">
  IVCon conception notes
</h3>

<h4>Overview:</h4>
<p>
  This chapter should be read by anyone who thinks that any
    of main conceptions of IVCON is wrong. If you like the way
    this tool is written, and you don't want to change scene
    data structure or object relations, you don't need
    to read this section.
</p>
<hr>

<h4>Concept:</h4>
<p>
  Format libraries are two (or more) methods written in separate
    file.
</p>
<h4>Explanation:</h4>
<p>
  Format libraries (tds.cpp,obj.cpp,...) are not separate objects,
    because it is easier and quicker to write two functions (one for
    reading and one for writing) than to create a new object.<br>
  There are no benefits for making each converter a separate object,
    it only enlarges the code and makes it unclear. It takes shorter
    to get familiar with the code when it is not fully object oriented.
</p>
<hr>

<h4>Concept:</h4>
<p>
  All scene information are stored separately in IVCON class, so there
    is not "Scene" object.
</p>
<h4>Explanation:</h4>
<p>
  This tool is intended to work with only one scene, so there are
    no reasons to close scene elements in object. This way makes
    accessing scene elements shorter and easier.
</p>
<hr>

<h4>Concept:</h4>
<p>
  There is no per-face material or texture mapping, only per-vertex.
</p>
<h4>Explanation:</h4>
<p>
  It took many days of discussion before this conception was accepted.<br>
  The main reason is to avoid programmer errors. This way is really
    best one. If a format uses per-face material mapping, the IVCON
    class contains proper functions to convert the mappings in both
    ways.
</p>
<hr>

<h4>Concept:</h4>
<p>
  There is no color list array, specific objects contain their own
    color entries.
</p>
<h4>Explanation:</h4>
<p>
  In many formats, there is a list of colors of vertices and lines.<br>
  In IVCON, colors are not on separate list, but inside specific
    lines or vertices. The class contains proper functions to copy
    colors from a list into each objects property and vice versa.
</p>

<hr>
<?php addGlobalFooterTags(); ?>

</div>

<BR>

</BODY>
</HTML>
 