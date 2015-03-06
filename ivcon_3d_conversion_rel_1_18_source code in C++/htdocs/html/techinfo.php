
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

<?php drawSidebar('sidebar_techinfo'); ?>

<div id="page_content">

<h4>
A single file format may support only limited elements of a scene, but
 the main program allows all formats to support:
</h4>

<ul>
<li>
Unlimited (32-bit, or 64-bit depending on system architecture) number
  of vertices (vertices are points in 3D space);
</li>
<li>
Unlimited (system-dependent) number of faces (face is a single closed area
  made of some vertices);
</li>
<li>
Unlimited (system-dependent) number of meshes/object (a mesh, or a object,
  is a set of faces making one element; mesh usually closes a volume);
</li>
<li>
Unlimited number of shapes/lines (lines are usually helpers in 3D scene);
</li>
<li>
Unlimited number of materials; diffuse, ambient and specular color
  in every material; alpha (transparency) is also supported;
</li>
<li>
Unlimited number of textures; alpha level for every texture;
</li>
<li>
Texture mapping (so-called UV mapping - defines how to place
  textures on meshes);
</li>
<li>
Three textures in every material: diffuse, diffuse alpha map and bump;
</li>
<li>
One default place of view and unlimited cameras
</li>
<li>
Environment parameters - color and texture; unlimited lights;
</li>
<li>
Program allows to merge scenes from different files;
  automatically fixes errors in data;
</li>
<li>
Basic data processing - face order reduction, small faces deletion;
</li>
</ul>

<hr>
<?php addGlobalFooterTags(); ?>

</div>

<BR>

</BODY>
</HTML>
 