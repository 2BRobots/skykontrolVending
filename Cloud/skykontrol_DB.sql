-- --------------------------------------------------------
-- Host:                         www.2brobots.com
-- Versión del servidor:         5.6.41-84.1 - Percona Server (GPL), Release 84.1, Revision b308619
-- SO del servidor:              Linux
-- HeidiSQL Versión:             9.4.0.5125
-- --------------------------------------------------------

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET NAMES utf8 */;
/*!50503 SET NAMES utf8mb4 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;


-- Volcando estructura de base de datos para brobotsc_skykontrol
CREATE DATABASE IF NOT EXISTS `brobotsc_skykontrol` /*!40100 DEFAULT CHARACTER SET utf8 COLLATE utf8_unicode_ci */;
USE `brobotsc_skykontrol`;

-- Volcando estructura para tabla brobotsc_skykontrol.controller_boards
CREATE TABLE IF NOT EXISTS `controller_boards` (
  `device_id` int(11) NOT NULL AUTO_INCREMENT,
  `mac_addr` text COLLATE utf8_unicode_ci NOT NULL,
  `user_id` text COLLATE utf8_unicode_ci NOT NULL,
  `password` text COLLATE utf8_unicode_ci NOT NULL,
  `firmware` text COLLATE utf8_unicode_ci,
  `message` text COLLATE utf8_unicode_ci NOT NULL,
  `company` text COLLATE utf8_unicode_ci NOT NULL,
  `cost` smallint(5) unsigned NOT NULL DEFAULT '1',
  `time` smallint(5) unsigned NOT NULL DEFAULT '5',
  `enabled` bit(1) NOT NULL DEFAULT b'1',
  `active` bit(1) NOT NULL DEFAULT b'1',
  `has_charLCD` bit(1) NOT NULL DEFAULT b'0',
  `ssid` text COLLATE utf8_unicode_ci,
  `pass` text COLLATE utf8_unicode_ci,
  `latitude` text COLLATE utf8_unicode_ci,
  `longitude` text COLLATE utf8_unicode_ci,
  `accuracy` text COLLATE utf8_unicode_ci,
  `manufacture_date` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `last_connection` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`device_id`)
) ENGINE=MyISAM AUTO_INCREMENT=16 DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

-- La exportación de datos fue deseleccionada.
-- Volcando estructura para tabla brobotsc_skykontrol.controller_events
CREATE TABLE IF NOT EXISTS `controller_events` (
  `device_id` int(11) NOT NULL DEFAULT '0',
  `message` text COLLATE utf8_unicode_ci,
  `datetime` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  KEY `device_id` (`device_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

-- La exportación de datos fue deseleccionada.
-- Volcando estructura para tabla brobotsc_skykontrol.power_meter
CREATE TABLE IF NOT EXISTS `power_meter` (
  `device_id` int(11) NOT NULL DEFAULT '0',
  `kwh` int(11) NOT NULL DEFAULT '0',
  `datetime` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  KEY `device_id` (`device_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci ROW_FORMAT=DYNAMIC;

-- La exportación de datos fue deseleccionada.
-- Volcando estructura para tabla brobotsc_skykontrol.resumes
CREATE TABLE IF NOT EXISTS `resumes` (
  `device_id` int(11) NOT NULL,
  `start_date` date DEFAULT NULL,
  `end_date` date DEFAULT NULL,
  `sales` int(11) DEFAULT NULL,
  `ammount` int(11) DEFAULT NULL,
  `power` int(11) DEFAULT NULL,
  `datetime` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  KEY `device_id` (`device_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

-- La exportación de datos fue deseleccionada.
-- Volcando estructura para tabla brobotsc_skykontrol.sales
CREATE TABLE IF NOT EXISTS `sales` (
  `device_id` int(11) NOT NULL DEFAULT '0',
  `price` int(11) NOT NULL DEFAULT '0',
  `method` text COLLATE utf8_unicode_ci,
  `datetime` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  KEY `device_id` (`device_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;

-- La exportación de datos fue deseleccionada.
/*!40101 SET SQL_MODE=IFNULL(@OLD_SQL_MODE, '') */;
/*!40014 SET FOREIGN_KEY_CHECKS=IF(@OLD_FOREIGN_KEY_CHECKS IS NULL, 1, @OLD_FOREIGN_KEY_CHECKS) */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
