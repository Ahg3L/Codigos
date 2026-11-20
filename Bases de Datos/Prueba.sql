CREATE DATABASE base_prueba
go

use base_prueba
go

create table empleados(
    Num_emp int primary key,
    Nom_emp varchar(20) not null,
    Fcont_emp DATE not null,
    Sex_exp varchar(1),
    Id_sal_emp int not null,
    Clave_pue_emp int not null,

    CONSTRAINT fK_id_sal_emp FOREIGN KEY (Id_sal_emp) REFERENCES Salud(Id_sal),  
    CONSTRAINT fK_pue_emp FOREIGN KEY (Clave_pue_emp) REFERENCES Puestos(Clave_pue)  

)


 