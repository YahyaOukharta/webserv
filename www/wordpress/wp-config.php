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
define( 'DB_HOST', '10.11.7.6' );

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
define( 'AUTH_KEY',         'nfA%`U~`[.dr.!7YwY`cK.}KV=/-.8oAd8%kbC)%,?1n%P%?/#jO:}9X9N>N9vIQ' );
define( 'SECURE_AUTH_KEY',  'zXC2cPRTbw?{Qj}}[SD+]TJ}!^J=m]e%TbsW:^J+4%E-0a7;)nHY[8HN{01_d28C' );
define( 'LOGGED_IN_KEY',    'h[Ra(Q-4KL@VhqTv(3.9@}:Y 0]i.j,ZE~-vnU4Y|>MB;wm~EjFhhDO|DE.6=Ua9' );
define( 'NONCE_KEY',        '@CrPL&8HN~g:W!+} $/FoqxSl2E5quD ^}glJA21~-%Y@*2eBHr eB61/})g,.3m' );
define( 'AUTH_SALT',        'd7FSlz]@w}`L06hS/^_~o<brs!]*uQVoqmVms]=}(,? =KzC/pXoS&5%,s(|q#m6' );
define( 'SECURE_AUTH_SALT', 'PV0*g[czQcJ8L2cnl%2,weH:gWIAtz&!V%>4^|abHR-)<WH(Qp/q qOO23#JCB0r' );
define( 'LOGGED_IN_SALT',   ',)D(.bC1~m/riM5A&zFD:X;CXWEN.u{bF>-9;E[G{StR|x v$97J3fzrtsib[RO:' );
define( 'NONCE_SALT',       'F*+S`6v0IKRU+SlTo4B6%IZl_Ev7}u=W*F$-YNG>xk*V:jRj+$o?sdw`tQ3,Kd-(' );

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
