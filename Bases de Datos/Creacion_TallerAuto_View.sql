CREATE DATABASE TallerAutos
ON
(
    NAME = TallerAutos_dat,
    FILENAME = 'C:\Users\migue\Downloads\MSSQL17.SQLEXPRESS\MSSQL\DATA\TallerAutos.mdf',
    SIZE = 10MB,
    MAXSIZE = 100MB,
    FILEGROWTH = 10MB
)
LOG ON
(
    NAME = TallerAutos_log,
    FILENAME = 'C:\Users\migue\Downloads\MSSQL17.SQLEXPRESS\MSSQL\DATA\TallerAutos.ldf',
    SIZE = 5MB,
    MAXSIZE = 50MB,
    FILEGROWTH = 5MB
);
GO

    use TallerAutos
    GO

    CREATE TABLE Impuestos (
        clave_imp SMALLINT PRIMARY KEY,
        nombre_imp VARCHAR(5) NOT NULL,
        valor_imp DECIMAL(3, 2) NOT NULL
    )
    GO
    INSERT into Impuestos VALUES
    (1,'AFORE', 0.06),
    (2,'IMSS',0.08),
    (3,'IVA',0.16),
    (4,'ISR',0.11)
    GO

    CREATE TABLE Tipo_Sangre (
        id_tipsan SMALLINT PRIMARY KEY,
        desc_tipsan VARCHAR(3) NOT NULL
    )
    GO
    INSERT INTO Tipo_Sangre VALUES
    (1, 'A+'),
    (2, 'AB+'),
    (3, 'AB-'),
    (4, 'B+'),
    (5, 'B-'),
    (6, 'O+'),
    (7, 'O-')
    GO

    CREATE TABLE Puestos (
        id_pue SMALLINT PRIMARY KEY,
        nom_pue VARCHAR(20) NOT NULL,
        sueldo_pue DECIMAL(8, 2) NOT NULL
    )
    GO
    INSERT INTO Puestos VALUES
    (1, N'MECANICO', 20000.00),
    (2, N'HOJALATERO', 14000.00),
    (3, N'PINTOR', 12000.00),
    (4, N'ELECTRICISTA', 17000.00),
    (5, N'TALACHERO', 7000.00),
    (6, N'SECRETARIA', 11000.00),
    (7, N'CONTADORA', 27000.00),
    (8, N'COMPRAS', 13000.00),
    (9, N'DISEÑADOR', 21000.00),
    (10, N'ABOGADO', 21000.00),
    (11, N'GERENTE', 40000.00),
    (12, N'PROGRAMADOR', 25000.00),
    (13, N'ATENCIÓN CLIENTES', 9000.00)
    GO

    CREATE TABLE Empleado (
        num_emp SMALLINT PRIMARY KEY,
        nom_emp VARCHAR(30) NOT NULL,
        apat_emp VARCHAR(15) NOT NULL,
        amat_emp VARCHAR(15) NOT NULL,
        sexo_emp VARCHAR(1) NOT NULL,
        fcon_emp DATE NOT NULL,
        tipsan_emp SMALLINT NOT NULL,
        puesto_emp SMALLINT NOT NULL,
        CONSTRAINT FK_TipoSangre FOREIGN KEY (tipsan_emp) REFERENCES Tipo_Sangre(id_tipsan),
        CONSTRAINT FK_puesto FOREIGN KEY (puesto_emp) REFERENCES Puestos(id_pue)
    )
    GO


    INSERT into Empleado VALUES
    (1, N'JUAN', N'DURAN', N'LOPEZ', 'M', '2005-08-15', 1, 1),
    (2, N'RAMIRO', N'GOMEZ', N'DALMA', 'M', '2006-02-28', 2, 2),
    (3, N'JESUS', N'JIMENEZ', N'RUIZ', 'M', '2009-07-15', 6, 4),
    (4, N'JULIAN', N'DEL VALLE', N'PANTOJA', 'M', '2005-08-15', 5, 5),
    (5, N'JORGE', N'GONZALEZ', N'LEYVA', 'M', '2021-12-30', 4, 1),
    (6, N'JOSE', N'REYES', N'LUNA', 'M', '2000-09-30', 7, 1),
    (7, N'ERNESTO', N'PEREZ', N'JUAREZ', 'M', '2017-11-15', 7, 2),
    (8, N'RAUL', N'RAMIREZ', N'FERNANDEZ', 'M', '2000-03-30', 2, 3),
    (9, N'ROSENDO', N'HERNANDEZ', N'GOMEZ', 'M', '2003-06-15', 4, 3),
    (10, N'FERNANDA', N'MATA', N'PERALTA', 'F', '2005-04-15', 6, 8),
    (11, N'LUISA', N'MORA', N'LOZANO', 'F', '2009-10-15', 3, 6),
    (12, N'JIMENA', N'MOLINA', N'MEJIA', 'F', '2019-01-30', 2, 7),
    (13, N'ISABELA', N'MEJIA', N'PALMA', 'F', '2013-05-15', 7, 9),
    (14, N'ARTURO', N'ARCE', N'GUERRA', 'M', '2000-03-30', 5, 9),
    (15, N'ROSA', N'MARQUEZ', N'HERRERA', 'F', '2009-10-15', 3, 6),
    (16, N'JUANA', N'AREVALO', N'ORDOÑEZ', 'F', '2003-06-15', 1, 13),
    (17, N'ARMANDO', N'BUENROSTRO', N'BLANCO', 'M', '2017-11-15', 5, 12),
    (18, N'EDGAR', N'VILLA', N'ASTURIAS', 'M', '2000-09-30', 4, 11),
    (19, N'LAURA', N'VALVERDE', N'GUERRERO', 'F', '2005-08-15', 4, 10),
    (20, N'NOE', N'ALMAZAN', N'ZAMORA', 'M', '2021-03-30', 1, 3)
    GO

    CREATE TABLE Nomina (
        num_emp_nom SMALLINT NOT NULL,
        faltas_emp_nom INT NOT NULL,
        CONSTRAINT FK_empleado FOREIGN KEY (num_emp_nom) REFERENCES Empleado(num_emp)
    )
    GO

    INSERT INTO Nomina VALUES
    (1, 0),
    (2, 1),
    (3, 0),
    (4, 0),
    (5, 2),
    (6, 0),
    (7, 0),
    (8, 0),
    (9, 3),
    (10, 0),
    (11, 2),
    (12, 0),
    (13, 0),
    (14, 1),
    (15, 1),
    (16, 0),
    (17, 1),
    (18, 0),
    (19, 0),
    (20, 2);
    GO


CREATE VIEW Vista_Nomina AS
SELECT
    e.num_emp                                   AS ID,
    e.nom_emp                                   AS NOMBRE,
    e.apat_emp                                  AS PATERNO,
    e.amat_emp                                  AS MATERNO,
    p.sueldo_pue                                AS SUELDO,
    p.nom_pue                                   AS PUESTO,
    p.id_pue                                    AS [Clave Puesto],
    e.fcon_emp                                  AS [FECHA CONTRATO],
    e.sexo_emp                                  AS SEXO,
    ts.desc_tipsan                              AS [TIPO SANGRE],
    n.faltas_emp_nom                            AS FALTAS,
    ROUND(c.descuento, 2)                       AS DESCUENTO,
    ROUND(c.bono, 2)                            AS BONO,
    ROUND(c.bruto, 2)                           AS [SUELDO BRUTO],
    ROUND(c.bruto * afore.valor_imp, 2)         AS AFORE,
    ROUND(c.bruto * imss.valor_imp, 2)          AS IMSS,
    ROUND(c.bruto * iva.valor_imp, 2)           AS IVA,
    ROUND(c.bruto * isr.valor_imp, 2)           AS ISR,
    ROUND(c.bruto * (1 - afore.valor_imp - imss.valor_imp - iva.valor_imp - isr.valor_imp), 2) AS [SUELDO NETO],
    ROUND(c.bruto * (afore.valor_imp + imss.valor_imp + iva.valor_imp + isr.valor_imp), 2)     AS [RETENCIÓN]
FROM Empleado e
JOIN Puestos p      ON p.id_pue = e.pue_emp
JOIN Tipo_Sangre ts ON ts.id_tipsan = e.tipsan_emp
JOIN Nomina n       ON n.num_emp_nom = e.num_emp
JOIN Impuestos afore ON afore.nombre_imp = 'AFORE'
JOIN Impuestos imss  ON imss.nombre_imp  = 'IMSS'
JOIN Impuestos iva   ON iva.nombre_imp   = 'IVA'
JOIN Impuestos isr   ON isr.nombre_imp   = 'ISR'
CROSS APPLY (SELECT
        DATEDIFF(YEAR, e.fcon_emp, GETDATE())
        - CASE WHEN DATEADD(YEAR, DATEDIFF(YEAR, e.fcon_emp, GETDATE()), e.fcon_emp) > CAST(GETDATE() AS DATE)
               THEN 1 ELSE 0 END AS antiguedad) a
CROSS APPLY (SELECT
        n.faltas_emp_nom * p.sueldo_pue / 30.0 AS descuento,
        p.sueldo_pue * CASE WHEN p.nom_pue = 'GERENTE' THEN 0
                            WHEN a.antiguedad > 15     THEN 0.07
                            WHEN a.antiguedad >= 10    THEN 0.05
                            ELSE 0 END AS bono) b
CROSS APPLY (SELECT b.descuento, b.bono,
                    p.sueldo_pue - b.descuento + b.bono AS bruto) c;
GO
