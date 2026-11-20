    use TallerAuto;
    Create procedure sp_reportEmp
    as 
        DECLARE
        @clave VARCHAR(6), @nombre VARCHAR(40), @fecha_cont DATE, @sexo VARCHAR(1), @numEmp SMALLINT, @cont SMALLINT, @id_tra SMALLINT
    BEGIN
        SELECT @numEmp= count(*) from Empleado
        set @cont=1
        while @cont <= @numEmp
        BEGIN
            SELECT @id_tra=num_emp, @nombre= nom_emp + ' '+ apat_emp+ ' '+ amat_emp,@fecha_cont=fcon_emp, @sexo=sexo_emp
            from Empleado
            where num_emp=@cont
            if @id_tra <10
                set @clave = 'EMP00'+ CONVERT(varchar, @id_tra)
                ELSE
                    set @clave='EMP0'+ CONVERT(varchar, @id_tra)
        PRINT @clave +' | '+@nombre+' | '+CONVERT(varchar, @fecha_cont)+' | '+@sexo
        set @cont =@cont+1
        end
    end;
    EXECUTE sp_reportEmp;