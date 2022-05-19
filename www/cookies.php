<html>
<body>
<?php
$cookie_name = "user";
$cookie_value = "John Doe";
if(!isset($_COOKIE[$cookie_name])) {
 

setcookie($cookie_name, $cookie_value, time() + (15), "/"); // 86400 = 1 day



  echo "Cookie named '" . $cookie_name . "' is not set!";
} else {
  echo "Cookie '" . $cookie_name . "' is set!<br>";
  echo "Value is: " . $_COOKIE[$cookie_name] . "!<br>";
}
?>

</body>
</html>