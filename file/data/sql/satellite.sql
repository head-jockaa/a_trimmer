CREATE TABLE `satellite` (
  `channel` int(11) NOT NULL,
  `physical` int(11) NOT NULL,
  `station_id` int(11) NOT NULL,
  PRIMARY KEY (`channel`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

INSERT INTO `satellite` VALUES
(1,15,50),
(2,15,51),
(3,15,180),
(4,13,181),
(5,1,182),
(6,1,183),
(7,3,184),
(8,13,185),
(9,3,186),
(10,9,187),
(11,9,188),
(12,9,189);
