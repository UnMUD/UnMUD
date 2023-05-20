-- TRIGGERS --

-- Verifica se o item adicionado no campo weaponId do Player é do tipo WEAPON
CREATE OR REPLACE FUNCTION checkWeaponType()
  RETURNS TRIGGER AS $checkWeaponType$
BEGIN
  IF NEW.weaponId IS NOT NULL THEN
    IF EXISTS (SELECT 1 FROM Item WHERE id = NEW.weaponId AND type != 'WEAPON') THEN
      RAISE EXCEPTION 'Invalid weapon type';
    END IF;
  END IF;

  RETURN NEW;
END;
$checkWeaponType$ LANGUAGE plpgsql;

DROP TRIGGER IF EXISTS triggerValidateWeaponTypeOnPlayer ON Player;
CREATE TRIGGER triggerValidateWeaponTypeOnPlayer
  BEFORE INSERT OR UPDATE OF weaponId ON Player
  FOR EACH ROW
  EXECUTE FUNCTION checkWeaponType();

DROP TRIGGER IF EXISTS triggerValidateWeaponTypeOnEnemy ON Enemy;
CREATE TRIGGER triggerValidateWeaponTypeOnEnemy
  BEFORE INSERT OR UPDATE OF weaponId ON Enemy
  FOR EACH ROW
  EXECUTE FUNCTION checkWeaponType();

-- Verifica se o item adicionado no campo weaponId do Player é do tipo ARMOR
CREATE OR REPLACE FUNCTION checkArmorType()
  RETURNS TRIGGER AS $checkArmorType$
BEGIN
  IF NEW.armorId IS NOT NULL THEN
    IF EXISTS (SELECT 1 FROM Item WHERE id = NEW.armorId AND type != 'ARMOR') THEN
      RAISE EXCEPTION 'Invalid armor type';
    END IF;
  END IF;

  RETURN NEW;
END;
$checkArmorType$ LANGUAGE plpgsql;

DROP TRIGGER IF EXISTS triggerValidateArmorTypeOnPlayer ON Player;
CREATE TRIGGER triggerValidateArmorTypeOnPlayer
  BEFORE INSERT OR UPDATE OF armorId ON Player
  FOR EACH ROW
  EXECUTE FUNCTION checkArmorType();

-- Verifica se o Map do tipo STORE possui uma storeId referenciada
CREATE OR REPLACE FUNCTION checkStoreId()
  RETURNS TRIGGER AS $checkStoreId$
BEGIN
  IF NEW.type = 'STORE' AND NEW.storeId IS NULL THEN
    RAISE EXCEPTION 'storeId cannot be null for Map of type STORE';
  END IF;

  RETURN NEW;
END;
$checkStoreId$ LANGUAGE plpgsql;

CREATE TRIGGER triggerValidateStoreId
  BEFORE INSERT OR UPDATE OF storeId ON Map
  FOR EACH ROW
  EXECUTE FUNCTION checkStoreId();
