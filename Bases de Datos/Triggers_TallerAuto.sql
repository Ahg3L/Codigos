USE TallerAutos;
GO

CREATE TABLE Bitacora_empleados (
    id_bitacora INT IDENTITY(1,1) PRIMARY KEY,
    num_emp SMALLINT,
    nombre_completo VARCHAR(70),
    sexo_emp VARCHAR(1),
    puesto_emp VARCHAR(20),
    sueldo_emp DECIMAL(8,2),
    fcon_emp DATE,
    usuario_reg VARCHAR(50),
    fecha_reg DATETIME,
    ip_maquina VARCHAR(50),
    accion VARCHAR(20)
);
GO

-- Trigger 1: Altas (AFTER INSERT)

CREATE TRIGGER tr_Empleado_Insert ON Empleado
AFTER INSERT
AS
BEGIN
    INSERT INTO Bitacora_empleados
    (num_emp, nombre_completo, sexo_emp, puesto_emp, sueldo_emp,
     fcon_emp, usuario_reg, fecha_reg, ip_maquina, accion)

    SELECT
        i.num_emp,
        i.nom_emp + ' ' + i.apat_emp + ' ' + i.amat_emp,
        i.sexo_emp,
        p.nom_pue,
        p.sueldo_pue,
        i.fcon_emp,
        SYSTEM_USER,
        GETDATE(),
        CAST(CONNECTIONPROPERTY('client_net_address') AS VARCHAR(50)),
        'CONTRATADO'

    FROM inserted i
    JOIN Puestos p ON i.puesto_emp = p.id_pue;
END;
GO


-- Trigger 2: Bajas (AFTER DELETE)

CREATE TRIGGER tr_Empleado_Delete ON Empleado
AFTER DELETE
AS
BEGIN
    INSERT INTO Bitacora_empleados
    (num_emp, nombre_completo, sexo_emp, puesto_emp, sueldo_emp,
     fcon_emp, usuario_reg, fecha_reg, ip_maquina, accion)

    SELECT
        d.num_emp,
        d.nom_emp + ' ' + d.apat_emp + ' ' + d.amat_emp,
        d.sexo_emp,
        p.nom_pue,
        p.sueldo_pue,
        d.fcon_emp,
        SYSTEM_USER,
        GETDATE(),
        CAST(CONNECTIONPROPERTY('client_net_address') AS VARCHAR(50)),
        'DESPEDIDO'

    FROM deleted d
    JOIN Puestos p ON d.puesto_emp = p.id_pue;
END;
GO


-- Trigger 3: Actualizaciones (AFTER UPDATE)

CREATE TRIGGER tr_Empleado_Update ON Empleado
AFTER UPDATE
AS
BEGIN
    INSERT INTO Bitacora_empleados
    (num_emp, nombre_completo, sexo_emp, puesto_emp, sueldo_emp,
     fcon_emp, usuario_reg, fecha_reg, ip_maquina, accion)

    SELECT
        i.num_emp,
        i.nom_emp + ' ' + i.apat_emp + ' ' + i.amat_emp,
        i.sexo_emp,
        p.nom_pue,
        p.sueldo_pue,
        i.fcon_emp,
        SYSTEM_USER,
        GETDATE(),
        CAST(CONNECTIONPROPERTY('client_net_address') AS VARCHAR(50)),
        'ACTUALIZADO'

    FROM inserted i
    JOIN Puestos p ON i.puesto_emp = p.id_pue;
END;
GO




USE TallerAutos;
GO

INSERT INTO Empleado VALUES
(21, 'MIGUEL', 'GARCIA', 'GONZALEZ',
 'M', '2026-11-05', 1, 12);
GO

-- Consultar la bitácora
SELECT * FROM Bitacora_empleados;
GO




USE TallerAutos;
GO

UPDATE Empleado
SET puesto_emp = 1
WHERE num_emp = 21;
GO

-- Consultar la bitácora
SELECT * FROM Bitacora_empleados;
GO





USE TallerAutos;
GO

DELETE FROM Empleado
WHERE num_emp = 21;
GO

-- Consultar la bitácora
SELECT * FROM Bitacora_empleados;
GO




SELECT
    id_bitacora,
    num_emp,
    nombre_completo,
    sexo_emp,
    puesto_emp,
    sueldo_emp,
    fcon_emp,
    usuario_reg,
    fecha_reg,
    ip_maquina,
    accion
FROM Bitacora_empleados
WHERE num_emp = 21
ORDER BY id_bitacora;
GO