<html>
<body>
<?php

session_start();

if(!isset($_SESSION["user"]))
  $_SESSION["user"] = "yahya";
else
  print_r($_SESSION);

?>

</body>
</html>