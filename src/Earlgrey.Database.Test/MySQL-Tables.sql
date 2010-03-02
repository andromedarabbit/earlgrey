CREATE DATABASE  IF NOT EXISTS`earlgrey_test` /*!40100 DEFAULT CHARACTER SET utf8 */;
USE `earlgrey_test`;
-- MySQL dump 10.13  Distrib 5.1.40, for Win32 (ia32)
--
-- Host: 127.0.0.1    Database: earlgrey_test
-- ------------------------------------------------------
-- Server version	5.1.43-community

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `simple_table_1`
--

DROP TABLE IF EXISTS `simple_table_1`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `simple_table_1` (
  `unique_key` int(11) NOT NULL,
  `name` varchar(45) DEFAULT '',
  `when` datetime DEFAULT NULL,
  PRIMARY KEY (`unique_key`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `simple_table_1`
--

LOCK TABLES `simple_table_1` WRITE;
/*!40000 ALTER TABLE `simple_table_1` DISABLE KEYS */;
INSERT INTO `simple_table_1` VALUES (0,'가','2010-02-27 21:49:43'),(1,'나','2011-02-27 21:49:44'),(2,'다','2012-02-27 21:49:45');
/*!40000 ALTER TABLE `simple_table_1` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `single_column_table`
--

DROP TABLE IF EXISTS `single_column_table`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `single_column_table` (
  `unique_key` int(11) NOT NULL,
  PRIMARY KEY (`unique_key`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `single_column_table`
--

LOCK TABLES `single_column_table` WRITE;
/*!40000 ALTER TABLE `single_column_table` DISABLE KEYS */;
INSERT INTO `single_column_table` VALUES (125);
/*!40000 ALTER TABLE `single_column_table` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2010-02-28 22:28:20
