
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<HTML>
<HEAD>
  <TITLE>IVCon project home</TITLE>
  <META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">

<?php
   require_once './include/basics.php';
   $home_path="./";
   addGlobalHeaderTags();
?>

</HEAD>

<?php
   require_once $home_path.'include/sidebar.php';
?>

<BODY TOPMARGIN=0 LEFTMARGIN=0 MARGINWIDTH=0 MARGINHEIGHT=0>

<div class="big_hdr" id="titlebar">IVCon 3D Graphics File Converter</div>

<div id="toplogo"><img <?php echo "src=\"".$home_path."/pictures/ivcon_icon_64.png\""; ?>
 ALT="IVCon logo" TITLE="IVCon logo"/></div>

<?php drawSidebar('sidebar_home'); ?>

<div id="page_content">


<p>
<b>IVCon</b> is an executable C++ program that can read and write a variety
of 3D graphics file formats, converting from one to the other.
</p>

<p>
This is extended version of John Burkardt's great, but buggy tool.
It Supports many 3D formats, and a lot of 3D scene elements.
</p>

<p>
Supported formats include 3DS, ASE, BYU, DXF, GMOD, HRC, IV, OBJ, OFF, POV, SMF,
STL/STLA/STLB, TEC, TRI/TRIA/TRIB, UCD, VLA, WRL/VRML, XGL and more.
</p>

<p>
The program supports many elements of 3D scene, but specific converters
may not support all of them. There are two possible reasons of that:
one is that the support is not fully programmed inside this tool, second
is that the format may not have possibilities to support the item.
Good example is the ASC format - which is an ASCII cloud of vertices,
and supports only bare points in 3D space.
</p>

<p>
The program is free and open source.
</p>

<hr>
<?php addGlobalFooterTags(); ?>

<P STYLE="font-size: small;">If you're using Internet Explorer, this site may look poor. Please change the browser, you&nbsp;will see the difference. Don't use IE - there are many reasons.</P>

</div>

<BR>

</BODY>
</HTML>
 