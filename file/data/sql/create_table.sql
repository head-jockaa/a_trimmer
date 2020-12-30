CREATE TABLE `area` (
  `id` int(11) NOT NULL,
  `name_jp` varchar(100) NOT NULL,
  `name_en` varchar(100) NOT NULL,
  `key1` tinyint(3) unsigned DEFAULT NULL,
  `key2` tinyint(3) unsigned DEFAULT NULL,
  `key3` tinyint(3) unsigned DEFAULT NULL,
  `key4` tinyint(3) unsigned DEFAULT NULL,
  `key5` tinyint(3) unsigned DEFAULT NULL,
  `key6` tinyint(3) unsigned DEFAULT NULL,
  `key7` tinyint(3) unsigned DEFAULT NULL,
  `key8` tinyint(3) unsigned DEFAULT NULL,
  `key9` tinyint(3) unsigned DEFAULT NULL,
  `key10` tinyint(3) unsigned DEFAULT NULL,
  `tv1` int(11) DEFAULT NULL,
  `tv2` int(11) DEFAULT NULL,
  `tv3` int(11) DEFAULT NULL,
  `tv4` int(11) DEFAULT NULL,
  `tv5` int(11) DEFAULT NULL,
  `tv6` int(11) DEFAULT NULL,
  `tv7` int(11) DEFAULT NULL,
  `tv8` int(11) DEFAULT NULL,
  `tv9` int(11) DEFAULT NULL,
  `tv10` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `rural` (
  `id` int(11) NOT NULL,
  `filepath` text NOT NULL,
  `tv1` int(11) DEFAULT NULL,
  `tv2` int(11) DEFAULT NULL,
  `tv3` int(11) DEFAULT NULL,
  `tv4` int(11) DEFAULT NULL,
  `tv5` int(11) DEFAULT NULL,
  `tv6` int(11) DEFAULT NULL,
  `tv7` int(11) DEFAULT NULL,
  `tv8` int(11) DEFAULT NULL,
  `tv9` int(11) DEFAULT NULL,
  `tv10` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `areacode` (
  `id` int(11) NOT NULL,
  `name_jp` varchar(100) NOT NULL,
  `name_en` varchar(100) NOT NULL,
  `button1` int(11) NOT NULL,
  `button2` int(11) NOT NULL,
  `button3` int(11) NOT NULL,
  `button4` int(11) NOT NULL,
  `button5` int(11) NOT NULL,
  `button6` int(11) NOT NULL,
  `button7` int(11) NOT NULL,
  `button8` int(11) NOT NULL,
  `button9` int(11) NOT NULL,
  `button10` int(11) NOT NULL,
  `button11` int(11) NOT NULL,
  `button12` int(11) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `cartoon` (
  `id` int(11) NOT NULL,
  `name_jp` varchar(200) NOT NULL,
  `name_en` varchar(200) NOT NULL,
  `search_query` text NOT NULL,
  `icon` int(11) NOT NULL,
  `red` tinyint(3) unsigned NOT NULL,
  `green` tinyint(3) unsigned NOT NULL,
  `blue` tinyint(3) unsigned NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `mountain` (
  `id` int(11) NOT NULL,
  `x` int(11) NOT NULL,
  `y` int(11) NOT NULL,
  `height` int(11) NOT NULL,
  `slope` int(11) NOT NULL,
  `range` int(11) NOT NULL,
  `volcano` tinyint(1) NOT NULL,
  `building` tinyint(4) NOT NULL,
  `note` text,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `satellite` (
  `channel` int(11) NOT NULL,
  `physical` int(11) NOT NULL,
  `station_id` int(11) NOT NULL,
  PRIMARY KEY (`channel`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `season` (
  `id` int(11) NOT NULL,
  `name_jp` varchar(100) NOT NULL,
  `name_en` varchar(100) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `station` (
  `id` int(11) NOT NULL,
  `icon` int(11) NOT NULL,
  `abbr` varchar(100) DEFAULT NULL,
  `name_jp` varchar(100) NOT NULL,
  `name_en` varchar(100) NOT NULL,
  `message_jp` text,
  `message_en` text,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `timeslot` (
  `id` int(11) NOT NULL,
  `season_id` int(11) NOT NULL,
  `week` tinyint(3) unsigned NOT NULL,
  `hour` tinyint(3) unsigned NOT NULL,
  `minute` tinyint(3) unsigned NOT NULL,
  `type` tinyint(3) unsigned NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `timetable` (
  `id` int(11) NOT NULL,
  `season_id` int(11) NOT NULL,
  `anime_id` int(11) NOT NULL,
  `station_id` int(11) NOT NULL,
  `week` tinyint(3) unsigned NOT NULL,
  `hour` tinyint(3) unsigned NOT NULL,
  `minute` tinyint(3) unsigned NOT NULL,
  `time` smallint(6) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `tower` (
  `id` int(11) NOT NULL,
  `name_jp` varchar(100) NOT NULL,
  `name_en` varchar(100) NOT NULL,
  `area_id` int(11) NOT NULL,
  `x` int(11) NOT NULL,
  `y` int(11) NOT NULL,
  `height` int(11) NOT NULL,
  `kw` float NOT NULL,
  `erp` float NOT NULL,
  `vertical` tinyint(1) NOT NULL,
  `bias` tinyint(3) unsigned NOT NULL,
  `ch1` int(11) DEFAULT NULL,
  `ch2` int(11) DEFAULT NULL,
  `ch3` int(11) DEFAULT NULL,
  `ch4` int(11) DEFAULT NULL,
  `ch5` int(11) DEFAULT NULL,
  `ch6` int(11) DEFAULT NULL,
  `ch7` int(11) DEFAULT NULL,
  `ch8` int(11) DEFAULT NULL,
  `ch9` int(11) DEFAULT NULL,
  `ch10` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

CREATE TABLE `town` (
  `id` int(11) NOT NULL,
  `name_jp` varchar(100) NOT NULL,
  `name_en` varchar(100) NOT NULL,
  `area_id` int(11) NOT NULL,
  `x` int(11) NOT NULL,
  `y` int(11) NOT NULL,
  `altitude` int(11) DEFAULT 0,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
