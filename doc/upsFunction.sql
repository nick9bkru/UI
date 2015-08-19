drop trigger t_ups_off on ui_ip;
drop function off_ups ( ) ;
CREATE OR REPLACE FUNCTION off_ups() RETURNS TRIGGER AS $$
BEGIN
    
    	  update ups set sost = 0 where ui = NEW.index ;
	RETURN NEW;
END;
$$ LANGUAGE plpgsql;



CREATE TRIGGER t_ups_off
AFTER UPDATE ON ui_ip 
FOR EACH ROW 
WHEN ( (OLD.sost IS DISTINCT FROM NEW.sost) and (NEW.sost = false ))
EXECUTE PROCEDURE off_ups ();