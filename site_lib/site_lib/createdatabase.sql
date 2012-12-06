drop database if exists gravicapa;
create database gravicapa;
use gravicapa;

create table Parametrs(
time TIMESTAMP not null default now(),
value FLOAT not null,
id_sensor BIGINT not null,
id INT AUTO_INCREMENT,
primary key (id)
);

create table Sensors(
time TIMESTAMP not null default now(),
auditory int default 0,
id_sensor BIGINT not null,
description VARCHAR(255),
ip_address  VARCHAR(16),
value ENUM('T','D','L'),
primary key (id_sensor)
);

create table Users(
id BIGINT not null AUTO_INCREMENT,
login varchar(255),
pass varchar(41),
primary key (id)
);
