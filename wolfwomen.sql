-- MySQL dump 10.13  Distrib 5.7.33, for Linux (x86_64)
--
-- Host: localhost    Database: wolfwomen
-- ------------------------------------------------------
-- Server version	5.7.33-0ubuntu0.16.04.1

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
-- Table structure for table `offLineOpt`
--

DROP TABLE IF EXISTS `offLineOpt`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `offLineOpt` (
  `userid` int(11) NOT NULL,
  `opt` int(11) NOT NULL,
  `sendid` int(11) NOT NULL,
  `chatMsg` varchar(256) DEFAULT NULL,
  `addContent` varchar(256) DEFAULT NULL,
  `addResult` tinyint(1) DEFAULT NULL,
  `goodSpaceid` int(11) DEFAULT NULL,
  `spaceComment` varchar(256) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `offLineOpt`
--

LOCK TABLES `offLineOpt` WRITE;
/*!40000 ALTER TABLE `offLineOpt` DISABLE KEYS */;
/*!40000 ALTER TABLE `offLineOpt` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_chatMsg`
--

DROP TABLE IF EXISTS `t_chatMsg`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_chatMsg` (
  `sendid` int(11) NOT NULL,
  `recvid` int(11) NOT NULL,
  `time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `content` varchar(256) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_chatMsg`
--

LOCK TABLES `t_chatMsg` WRITE;
/*!40000 ALTER TABLE `t_chatMsg` DISABLE KEYS */;
/*!40000 ALTER TABLE `t_chatMsg` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_comment`
--

DROP TABLE IF EXISTS `t_comment`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_comment` (
  `spaceid` int(11) NOT NULL,
  `userid` int(11) NOT NULL,
  `content` varchar(256) DEFAULT NULL,
  `time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_comment`
--

LOCK TABLES `t_comment` WRITE;
/*!40000 ALTER TABLE `t_comment` DISABLE KEYS */;
INSERT INTO `t_comment` VALUES (1,2,'buzhidao','2023-08-02 13:02:13'),(5,1,'zaima1','2023-08-05 08:14:35'),(5,1,'zaima1','2023-08-05 08:17:23'),(4,1,'哈哈哈哈','2023-08-05 08:20:20'),(5,1,'在吗','2023-08-05 08:29:55'),(3,1,'我也不认识','2023-08-05 08:36:19'),(5,1,'test','2023-08-05 08:49:11'),(5,2,'什么东西','2023-08-05 11:23:00');
/*!40000 ALTER TABLE `t_comment` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_friend`
--

DROP TABLE IF EXISTS `t_friend`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_friend` (
  `id_a` int(11) NOT NULL,
  `id_b` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_friend`
--

LOCK TABLES `t_friend` WRITE;
/*!40000 ALTER TABLE `t_friend` DISABLE KEYS */;
INSERT INTO `t_friend` VALUES (1,2),(2,1);
/*!40000 ALTER TABLE `t_friend` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_historyOpt`
--

DROP TABLE IF EXISTS `t_historyOpt`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_historyOpt` (
  `userid` int(11) NOT NULL,
  `opt` int(11) NOT NULL,
  `sendid` int(11) NOT NULL,
  `chatMsg` varchar(256) DEFAULT NULL,
  `addContent` varchar(256) DEFAULT NULL,
  `addResult` tinyint(1) DEFAULT NULL,
  `goodSpaceid` int(11) DEFAULT NULL,
  `spaceComment` varchar(256) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_historyOpt`
--

LOCK TABLES `t_historyOpt` WRITE;
/*!40000 ALTER TABLE `t_historyOpt` DISABLE KEYS */;
INSERT INTO `t_historyOpt` VALUES (1,4,2,NULL,NULL,NULL,1,NULL),(3,4,2,NULL,NULL,NULL,5,NULL),(3,4,1,NULL,NULL,NULL,4,NULL),(3,4,1,NULL,NULL,NULL,5,NULL),(3,4,2,NULL,NULL,NULL,5,NULL),(3,4,2,NULL,NULL,NULL,4,NULL),(2,4,2,NULL,NULL,NULL,3,NULL),(3,4,1,NULL,NULL,NULL,4,NULL),(2,4,1,NULL,NULL,NULL,3,NULL),(3,4,1,NULL,NULL,NULL,5,NULL),(3,4,1,NULL,NULL,NULL,5,NULL),(1,4,1,NULL,NULL,NULL,1,NULL),(1,4,1,NULL,NULL,NULL,2,NULL),(3,4,1,NULL,NULL,NULL,5,NULL),(3,4,1,NULL,NULL,NULL,5,NULL),(3,4,1,NULL,NULL,NULL,5,NULL);
/*!40000 ALTER TABLE `t_historyOpt` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_space`
--

DROP TABLE IF EXISTS `t_space`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_space` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `content` varchar(256) DEFAULT NULL,
  `userid` int(11) DEFAULT NULL,
  `good` int(11) DEFAULT '0',
  `tui` int(11) DEFAULT '0',
  `commentNum` int(11) DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_space`
--

LOCK TABLES `t_space` WRITE;
/*!40000 ALTER TABLE `t_space` DISABLE KEYS */;
INSERT INTO `t_space` VALUES (1,'2023-08-05 08:33:17','hello,my name is lili!',1,2,1,1),(2,'2023-08-05 08:08:45','nihao',1,1,1,0),(3,'2023-08-05 08:08:39','我不认识你',2,2,0,1),(4,'2023-08-05 08:20:20','我认识你',3,1,0,1),(5,'2023-08-05 08:49:47','why',3,0,1,5);
/*!40000 ALTER TABLE `t_space` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_spaceOpt`
--

DROP TABLE IF EXISTS `t_spaceOpt`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_spaceOpt` (
  `spaceid` int(11) NOT NULL,
  `userid` int(11) NOT NULL,
  `good` tinyint(1) DEFAULT NULL,
  `tui` tinyint(1) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_spaceOpt`
--

LOCK TABLES `t_spaceOpt` WRITE;
/*!40000 ALTER TABLE `t_spaceOpt` DISABLE KEYS */;
INSERT INTO `t_spaceOpt` VALUES (1,2,1,1),(2,2,0,0),(5,2,0,0),(4,1,0,0),(5,1,0,1),(4,2,1,0),(3,2,1,0),(3,1,1,0),(2,1,1,1),(1,1,1,0);
/*!40000 ALTER TABLE `t_spaceOpt` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `t_user`
--

DROP TABLE IF EXISTS `t_user`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `t_user` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `username` varchar(20) NOT NULL,
  `password` varchar(100) DEFAULT NULL,
  `sex` varchar(5) DEFAULT NULL,
  `icon` int(11) DEFAULT NULL,
  `name` varchar(25) DEFAULT NULL,
  `level` int(11) DEFAULT NULL,
  `gameNum` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=19 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `t_user`
--

LOCK TABLES `t_user` WRITE;
/*!40000 ALTER TABLE `t_user` DISABLE KEYS */;
INSERT INTO `t_user` VALUES (1,'worldd','f9b77d488ca8463fa2f8bdb44863159d','男',1,'出发',5,0),(2,'worlddd','f9b77d488ca8463fa2f8bdb44863159d','女',5,'天线宝宝',1,0),(3,'worldddd','f9b77d488ca8463fa2f8bdb44863159d','男',8,'别错了！',1,0),(4,'quanquan','f8703735dc06609061882dbf91aae855','女',4,'圈圈',1,0),(5,'worldda','f9b77d488ca8463fa2f8bdb44863159d','男',5,'hahah',1,0),(6,'longname','355b7934b39b0a8cf2a6ed67352d3119','女',6,'戳戳',1,0),(7,'worlddb','f9b77d488ca8463fa2f8bdb44863159d','男',7,'hasdo',1,0),(8,'worlddc','f9b77d488ca8463fa2f8bdb44863159d','男',8,'wdqf',1,0),(9,'worldde','f9b77d488ca8463fa2f8bdb44863159d','男',9,'hahahhahahhaha',1,0);
/*!40000 ALTER TABLE `t_user` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2023-08-05 20:04:32
