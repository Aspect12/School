-- phpMyAdmin SQL Dump
-- version 5.2.1
-- https://www.phpmyadmin.net/
--
-- Host: 127.0.0.1
-- Generation Time: Jun 14, 2024 at 12:34 AM
-- Server version: 10.4.32-MariaDB
-- PHP Version: 8.2.12

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `employee_db`
--

-- --------------------------------------------------------

--
-- Table structure for table `access_accounts`
--

CREATE TABLE `access_accounts` (
  `id` int(11) NOT NULL,
  `username` varchar(45) NOT NULL,
  `password` varchar(45) NOT NULL,
  `isAdmin` tinyint(1) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Dumping data for table `access_accounts`
--

INSERT INTO `access_accounts` (`id`, `username`, `password`, `isAdmin`) VALUES
(2, 'user', '1', 0),
(4, 'test', 'test', 0),
(5, 'asd', 'asd', 1),
(7, 'test1', 'test1', 0),
(9, 'test13', 'test14', 0),
(12, 'asddsadas', 'sadtest1', 0),
(17, 'root', '1234', 1),
(18, 'username', 'password', 0),
(19, 'registered', 'registered', 0),
(20, 'asde', 'asd', 0),
(21, 'testt', 'testt', 1);

-- --------------------------------------------------------

--
-- Table structure for table `employees`
--

CREATE TABLE `employees` (
  `id` int(11) NOT NULL,
  `name` varchar(50) NOT NULL,
  `surname` varchar(50) NOT NULL,
  `father_name` varchar(50) NOT NULL,
  `mother_name` varchar(50) NOT NULL,
  `TIN` int(9) NOT NULL,
  `SSN` int(11) NOT NULL,
  `address` varchar(50) NOT NULL,
  `email_1` varchar(50) NOT NULL,
  `email_2` varchar(50) NOT NULL,
  `employee_type` tinyint(1) NOT NULL,
  `vacancies` int(11) NOT NULL,
  `work_hours` int(11) NOT NULL,
  `salary_type` tinyint(1) NOT NULL,
  `kid_count` int(11) NOT NULL,
  `marital_status` tinyint(1) NOT NULL,
  `education_level` varchar(255) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

-- --------------------------------------------------------

--
-- Table structure for table `employee_days_off`
--

CREATE TABLE `employee_days_off` (
  `id` int(11) NOT NULL,
  `employee_id` int(11) NOT NULL,
  `days_off` int(11) NOT NULL,
  `days_off_date` date NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

-- --------------------------------------------------------

--
-- Table structure for table `employee_hr`
--

CREATE TABLE `employee_hr` (
  `id` int(11) NOT NULL,
  `employee_id` int(11) NOT NULL,
  `hire_date` date NOT NULL,
  `fire_date` date NOT NULL,
  `position` varchar(50) NOT NULL,
  `department` varchar(50) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

-- --------------------------------------------------------

--
-- Table structure for table `employee_phones`
--

CREATE TABLE `employee_phones` (
  `id` int(11) NOT NULL,
  `phone` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

-- --------------------------------------------------------

--
-- Table structure for table `employee_salaries`
--

CREATE TABLE `employee_salaries` (
  `id` int(11) NOT NULL,
  `employee_id` int(11) NOT NULL,
  `salary` float NOT NULL,
  `salary_date` date NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

-- --------------------------------------------------------

--
-- Table structure for table `employee_sick_days`
--

CREATE TABLE `employee_sick_days` (
  `id` int(11) NOT NULL,
  `employee_id` int(11) NOT NULL,
  `sick_days` int(11) NOT NULL,
  `sick_days_date` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_general_ci;

--
-- Indexes for dumped tables
--

--
-- Indexes for table `access_accounts`
--
ALTER TABLE `access_accounts`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `employees`
--
ALTER TABLE `employees`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `employee_days_off`
--
ALTER TABLE `employee_days_off`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `employee_salaries`
--
ALTER TABLE `employee_salaries`
  ADD PRIMARY KEY (`id`);

--
-- Indexes for table `employee_sick_days`
--
ALTER TABLE `employee_sick_days`
  ADD PRIMARY KEY (`id`);

--
-- AUTO_INCREMENT for dumped tables
--

--
-- AUTO_INCREMENT for table `access_accounts`
--
ALTER TABLE `access_accounts`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=22;

--
-- AUTO_INCREMENT for table `employees`
--
ALTER TABLE `employees`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=2;

--
-- AUTO_INCREMENT for table `employee_days_off`
--
ALTER TABLE `employee_days_off`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `employee_salaries`
--
ALTER TABLE `employee_salaries`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT for table `employee_sick_days`
--
ALTER TABLE `employee_sick_days`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
