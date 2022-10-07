-- phpMyAdmin SQL Dump
-- version 5.2.0
-- https://www.phpmyadmin.net/
--
-- Servidor: 127.0.0.1
-- Tiempo de generación: 23-09-2022 a las 05:10:10
-- Versión del servidor: 10.4.24-MariaDB
-- Versión de PHP: 7.4.29

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Base de datos: `bd_arduino`
--
-- -----------------------------------------------------
-- Schema bd_arduino
-- -----------------------------------------------------
CREATE SCHEMA IF NOT EXISTS `bd_arduino` DEFAULT CHARACTER SET utf8 ;
USE `bd_arduino` ;

--
-- Estructura de tabla para la tabla `nodos` 1
--

CREATE TABLE `nodos` (
  `id_nodo` int(11) NOT NULL,
  `nombre` varchar(70) COLLATE utf8_bin NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;

--
-- Volcado de datos para la tabla `nodos`
--

INSERT INTO `nodos` (`id_nodo`, `nombre`) VALUES
(1, 'Laboratorios de Ingenieria'),
(2, 'Laboratorios Especializados'),
(3, 'Planta Electrica'),
(4, 'Bloque Tecnologico'),
(5, 'Biblioteca'),
(6, 'Bloque 2'),
(7, 'Administracion'),
(8, 'Facultad de Artes'),
(9, 'Coliseo'),
(10, 'Laboratorios de Docencia');

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `seguimientos` 2
--

CREATE TABLE `seguimientos` (
  `id_seguimiento` int(11) NOT NULL,
  `nodo_id` int(11) NOT NULL,
  `fecha_seguimiento` TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `valor_voltaje` varchar(15) COLLATE utf8_bin NOT NULL,
  `valor_corriente` varchar(15) COLLATE utf8_bin NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;

--
-- Volcado de datos para la tabla `seguimientos`
--

INSERT INTO seguimientos(`id_seguimiento`,`nodo_id`, `valor_voltaje`, `valor_corriente`) VALUES 
(1,1,49,23),
(2,1,258,21),
(3,1,44,10),
(4,1,120,25),
(5,1,120,25),
(6,1,112,13),
(7,1,110,10),
(8,1,101,11),
(9,1,56,07),
(10,1,80,22);
-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `tipos_falla` 3
--

CREATE TABLE `tipos_falla` (
  `id_tipo_falla` int(11) NOT NULL,
  `nombre` varchar(45) COLLATE utf8_bin DEFAULT NULL,
  `detalle` varchar(255) COLLATE utf8_bin DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;

--
-- Volcado de datos para la tabla `tipos_falla`
--

INSERT INTO `tipos_falla` (`id_tipo_falla`, `nombre`, `detalle`) VALUES
(1, 'caida voltaje 1', 'Voltaje entre 0 y 50'),
(2, 'caida voltaje 2', 'Voltaje entre 51 y 100'),
(3, 'subida voltaje 1', 'Voltaje entre 121 y 150'),
(4, 'subida voltaje 2', 'Voltaje entre 151 y 170'),
(5, 'subida voltaje 3', 'Voltaje entre 170 y 2000'),
(6, 'subida voltaje 4', 'Voltaje mas de 200');

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `fallas` 4
--

CREATE TABLE `fallas` (
  `id_falla` int(11) NOT NULL,
  `nodo_id` int(11) NOT NULL,
  `tipo_falla_id` int(11) NOT NULL,
  `fecha_falla` TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `valor_voltaje` varchar(15) COLLATE utf8_bin NOT NULL,
  `valor_corriente` varchar(15) COLLATE utf8_bin NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;

--
-- Volcado de datos para la tabla `fallas`
--

INSERT INTO `fallas` (`id_falla`,`nodo_id`, `tipo_falla_id`, `valor_voltaje`, `valor_corriente`) VALUES
(1,1,3,49,23),
(2,1,2,258,21),
(3,1,1,44,10),
(4,1,2,90,25),
(5,1,3,130,25),
(6,1,5,240,13),
(7,1,6,250,10),
(8,1,4,200,11),
(9,1,2,56,07),
(10,1,3,80,22);

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `roles` 5
--

CREATE TABLE `roles` (
  `id_rol` int(11) NOT NULL,
  `nombre` varchar(45) COLLATE utf8_bin NOT NULL,
  `permiso` varchar(45) COLLATE utf8_bin NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;

--
-- Volcado de datos para la tabla `roles`
--

INSERT INTO `roles` (`id_rol`, `nombre`, `permiso`) VALUES
(1, 'SuperUsuario', '0'),
(2, 'Administrador', '0'),
(3, 'Operador', '1'),
(4, 'Visitante', '2');

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `usuarios` 6
--

CREATE TABLE `usuarios` (
  `id_usuario` int(11) NOT NULL,
  `nombre` varchar(45) COLLATE utf8_bin NOT NULL,
  `email` varchar(20) COLLATE utf8_bin NOT NULL,
  `password` varchar(200) COLLATE utf8_bin NOT NULL,
  `rol_id` int(11) NOT NULL,
  `cedula` varchar(45) COLLATE utf8_bin NOT NULL,	
  `telefono` varchar(15) COLLATE utf8_bin NOT NULL,
  `direccion` varchar(100) COLLATE utf8_bin NOT NULL,
  `estado` tinyint(1) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;

--
-- Volcado de datos para la tabla `usuarios`
--

INSERT INTO `usuarios` (`id_usuario`, `nombre`, `email`, `password`, `rol_id`, `cedula`, `telefono`,`direccion`,`estado`) VALUES
(1, 'Prueba', 'pr@pr.com', '$2y$10$idSD75VC70UzN7PS4DO8EegFKDapswtiZ59aR/MqQ26p4aaZ5/dQ2', 3, '123', 'dir user1 prueba', '3007808649',0),
(2, 'Prueba2', 'pr2@pr2.com', '$2y$10$02NM4klN9l57AW.1ouRbOOJja2qsmiwAYLMSDHpp..tOXq.oQWe6q', 2, '456', 'dir user2', '3007808649',1),
(4, 'a', 'a@a.com', '$2y$10$2fFIUJhmNpN7iGArVofxa.v/iWtYlDNeJj9uZSzhENn0xEH.r.n7S', 3, '246', 'dir user3', '3007808649',1);

-- --------------------------------------------------------


--
-- Estructura de tabla para la tabla `atienden` 7
--

CREATE TABLE `atienden` (
  `id_atiende` int(11) NOT NULL,
  `nodo_id` int(11) NOT NULL,
  `usuario_id` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;

--
-- Volcado de datos para la tabla `atienden`
--

INSERT INTO `atienden` (`id_atiende`, `nodo_id`, `usuario_id`) VALUES
(1, 1, 1),
(2, 2, 1),
(3, 1, 2),
(4, 2, 2);

-- --------------------------------------------------------

--
-- Estructura de tabla para la tabla `atenciones` 8
--

CREATE TABLE `atenciones` (
  `id_atencion` int(11) NOT NULL,
  `falla_id` int(11) NOT NULL,	  
  `usuario_id` int(11) NOT NULL,
  `fecha_atencion` TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
  `reporte` text
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_bin;

--
-- Volcado de datos para la tabla `atienden`
--

INSERT INTO `atenciones` (`id_atencion`, `falla_id`, `usuario_id`,`reporte`) VALUES
(1, 1, 1,'se arreglo sola');
-- --------------------------------------------------------

--
-- Índices para tablas volcadas
--

--
-- Indices de la tabla `atienden`1
--
ALTER TABLE `atienden`
  ADD PRIMARY KEY (`id_atiende`),
  ADD KEY `usuario_id` (`usuario_id`),
  ADD KEY `nodo_id` (`nodo_id`);

--
-- Indices de la tabla `seguimientos` 2
--
ALTER TABLE `seguimientos`
  ADD PRIMARY KEY (`id_seguimiento`),
  ADD KEY `nodo_id` (`nodo_id`);

--
-- Indices de la tabla `fallas`3
--
ALTER TABLE `fallas`
  ADD PRIMARY KEY (`id_falla`),
  ADD KEY `nodo_id` (`nodo_id`),
  ADD KEY `tipo_falla_id` (`tipo_falla_id`);

--
-- Indices de la tabla `atenciones`4
--
ALTER TABLE `atenciones`
  ADD PRIMARY KEY (`id_atencion`),
  ADD KEY `usuario_id` (`usuario_id`),
  ADD KEY `falla_id` (`falla_id`);
--

--
-- Indices de la tabla `nodos`5
--
ALTER TABLE `nodos`
  ADD PRIMARY KEY (`id_nodo`);

--
-- Indices de la tabla `roles`6
--
ALTER TABLE `roles`
  ADD PRIMARY KEY (`id_rol`);

--
-- Indices de la tabla `tipos_falla`7
--
ALTER TABLE `tipos_falla`
  ADD PRIMARY KEY (`id_tipo_falla`);

--
-- Indices de la tabla `usuarios`8
--
ALTER TABLE `usuarios`
  ADD PRIMARY KEY (`id_usuario`),
  ADD KEY `rol_id` (`rol_id`);

--
-- AUTO_INCREMENT de las tablas volcadas
--

--
-- AUTO_INCREMENT de la tabla `atienden` 1
--
ALTER TABLE `atienden`
  MODIFY `id_atiende` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT de la tabla `seguimientos` 2
--
ALTER TABLE `seguimientos`
  MODIFY `id_seguimiento` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT de la tabla `fallas` 3
--
ALTER TABLE `fallas`
  MODIFY `id_falla` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT de la tabla `atenciones` 4
--
ALTER TABLE `atenciones`
  MODIFY `id_atencion` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT de la tabla `nodos` 5
--
ALTER TABLE `nodos`
  MODIFY `id_nodo` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=11;

--
-- AUTO_INCREMENT de la tabla `roles` 6
--
ALTER TABLE `roles`
  MODIFY `id_rol` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=5;

--
-- AUTO_INCREMENT de la tabla `tipos_falla` 7
--
ALTER TABLE `tipos_falla`
  MODIFY `id_tipo_falla` int(11) NOT NULL AUTO_INCREMENT;

--
-- AUTO_INCREMENT de la tabla `usuarios` 8
--
ALTER TABLE `usuarios`
  MODIFY `id_usuario` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=32;

--
-- Restricciones para tablas volcadas
--

--
-- Filtros para la tabla `atienden` 1
--
ALTER TABLE `atienden`
  ADD CONSTRAINT `atienden_ibfk_1` FOREIGN KEY (`usuario_id`) REFERENCES `usuarios` (`id_usuario`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  ADD CONSTRAINT `atienden_ibfk_2` FOREIGN KEY (`nodo_id`) REFERENCES `nodos` (`id_nodo`) ON DELETE NO ACTION ON UPDATE NO ACTION;


--
-- Filtros para la tabla `seguimientos`2
--
ALTER TABLE `seguimientos`
  ADD CONSTRAINT `seguimientos_ibfk_1` FOREIGN KEY (`nodo_id`) REFERENCES `nodos` (`id_nodo`) ON DELETE NO ACTION ON UPDATE NO ACTION;

--
-- Filtros para la tabla `fallas`3
--
ALTER TABLE `fallas`
  ADD CONSTRAINT `fallas_ibfk_1` FOREIGN KEY (`nodo_id`) REFERENCES `nodos` (`id_nodo`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  ADD CONSTRAINT `fallas_ibfk_2` FOREIGN KEY (`tipo_falla_id`) REFERENCES `tipos_falla` (`id_tipo_falla`) ON DELETE NO ACTION ON UPDATE NO ACTION;

--
-- Filtros para la tabla `atenciones`4
--
ALTER TABLE `atenciones`
  ADD CONSTRAINT `atenciones_ibfk_1` FOREIGN KEY (`usuario_id`) REFERENCES `usuarios` (`id_usuario`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  ADD CONSTRAINT `atenciones_ibfk_2` FOREIGN KEY (`falla_id`) REFERENCES `fallas` (`id_falla`) ON DELETE NO ACTION ON UPDATE NO ACTION;

--
-- Filtros para la tabla `usuarios`5
--
ALTER TABLE `usuarios`
  ADD CONSTRAINT `usuarios_ibfk_1` FOREIGN KEY (`rol_id`) REFERENCES `roles` (`id_rol`) ON DELETE NO ACTION ON UPDATE NO ACTION;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
