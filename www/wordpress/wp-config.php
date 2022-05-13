<?php
/**
 * The base configuration for WordPress
 *
 * The wp-config.php creation script uses this file during the installation.
 * You don't have to use the web site, you can copy this file to "wp-config.php"
 * and fill in the values.
 *
 * This file contains the following configurations:
 *
 * * Database settings
 * * Secret keys
 * * Database table prefix
 * * ABSPATH
 *
 * @link https://wordpress.org/support/article/editing-wp-config-php/
 *
 * @package WordPress
 */

// ** Database settings - You can get this info from your web host ** //
/** The name of the database for WordPress */
define( 'DB_NAME', 'wordpress' );

/** Database username */
define( 'DB_USER', 'root' );

/** Database password */
define( 'DB_PASSWORD', 'root' );

/** Database hostname */
define( 'DB_HOST', '127.0.01' );

/** Database charset to use in creating database tables. */
define( 'DB_CHARSET', 'utf8mb4' );

/** The database collate type. Don't change this if in doubt. */
define( 'DB_COLLATE', '' );

/**#@+
 * Authentication unique keys and salts.
 *
 * Change these to different unique phrases! You can generate these using
 * the {@link https://api.wordpress.org/secret-key/1.1/salt/ WordPress.org secret-key service}.
 *
 * You can change these at any point in time to invalidate all existing cookies.
 * This will force all users to have to log in again.
 *
 * @since 2.6.0
 */
define( 'AUTH_KEY',         ':iHxYE1`8qUdb9vnl*_rwwUuR~r#Cx&~={Yk0t+>*mz[;;V`lyNpufS~6MPRPQNv' );
define( 'SECURE_AUTH_KEY',  'b1~V;v;Z{_q] b1yang(0V mw 1ApPkz#?$+p%l4n?FpGQ9pv+LCSD#WH_D<$0DP' );
define( 'LOGGED_IN_KEY',    '3P+5_r}8+mA7@xI?;*-c+skmzV{a(<ubPGRV]&90Lu/Gtd~c-/hE7o8x*uQdqR@d' );
define( 'NONCE_KEY',        'ay)YMZt>>:7`P2<ThR&amSyKI<u-b2zca>nz)&fm]s*HdT!Vll*[:=A_`,3DQKY:' );
define( 'AUTH_SALT',        ':h*|)cxq}6GKDv80vq*aF]h@S$sTR@5kV(<}S56~q<rQ0i&8ne}w_)5%l7oR]}q0' );
define( 'SECURE_AUTH_SALT', '}u[:`iR~43z&FUw+1SHo4_ew%PbjDje8?RM5@QG/Xu?s9 A!xA.iA1a+GC7wET^b' );
define( 'LOGGED_IN_SALT',   'K!e:L<@#NdsdRlWqJg-2{28eAXUqSRP?7~jMBIy!S.T|i0g;+Y9wf2{J%Q/e`G3L' );
define( 'NONCE_SALT',       '<pg`]e(O[p4JUV:@#t:e E>D%l1F4eCF,-w!9wW-UE}km,d9ISp5t=}QYsoqnj:.' );

/**#@-*/

/**
 * WordPress database table prefix.
 *
 * You can have multiple installations in one database if you give each
 * a unique prefix. Only numbers, letters, and underscores please!
 */
$table_prefix = 'wp_';

/**
 * For developers: WordPress debugging mode.
 *
 * Change this to true to enable the display of notices during development.
 * It is strongly recommended that plugin and theme developers use WP_DEBUG
 * in their development environments.
 *
 * For information on other constants that can be used for debugging,
 * visit the documentation.
 *
 * @link https://wordpress.org/support/article/debugging-in-wordpress/
 */
define( 'WP_DEBUG', false );

/* Add any custom values between this line and the "stop editing" line. */



/* That's all, stop editing! Happy publishing. */

/** Absolute path to the WordPress directory. */
if ( ! defined( 'ABSPATH' ) ) {
	define( 'ABSPATH', __DIR__ . '/' );
}

/** Sets up WordPress vars and included files. */
require_once ABSPATH . 'wp-settings.php';
