from django.db import migrations

class Migration(migrations.Migration):

    initial = True

    prikaz = "CREATE TABLE IF NOT EXISTS ov.companies(  " \
    "	cin bigint PRIMARY KEY,  " \
    "	name character varying, " \
    "	br_section character varying, " \
    "	address_line character varying, " \
    "	last_update timestamp without time zone, " \
    "	created_at timestamp without time zone, " \
    "	updated_at timestamp without time zone " \
    "); " \
    "	 " \
    " " \
    "INSERT INTO ov.companies (cin, name, br_section, address_line, last_update, created_at, updated_at) " \
    "        SELECT cin, corporate_body_name, br_section, address_line, last_update, created_at, updated_at FROM( " \
    "            SELECT cin, corporate_body_name, br_section, address_line, updated_at last_update, NOW()::timestamp created_at, NOW()::timestamp updated_at, row_number() OVER( " \
    "                PARTITION BY cin " \
    "                ORDER BY updated_at DESC " \
    "            ) rw_number " \
    "            FROM ov.or_podanie_issues WHERE cin IS NOT NULL " \
    "        ) query_1 " \
    "        WHERE rw_number = 1 " \
    "    ON CONFLICT (cin) DO NOTHING; " \
    " " \
    "    INSERT INTO ov.companies (cin, name, br_section, address_line, last_update, created_at, updated_at) " \
    "        SELECT cin, corporate_body_name, br_section, address_line, last_update, created_at, updated_at FROM( " \
    "            SELECT cin, corporate_body_name, br_section, street || ' ' || building_number || ', ' || postal_code || ' ' || city as address_line, updated_at last_update, NOW()::timestamp created_at, NOW()::timestamp updated_at, row_number() OVER( " \
    "                PARTITION BY cin " \
    "                ORDER BY updated_at DESC " \
    "            ) rw_number " \
    "            FROM ov.likvidator_issues WHERE cin IS NOT NULL " \
    "        ) query_2 " \
    "        WHERE rw_number = 1 " \
    "    ON CONFLICT (cin) DO NOTHING; " \
    " " \
    "    INSERT INTO ov.companies (cin, name, address_line, last_update, created_at, updated_at) " \
    "        SELECT cin, corporate_body_name, address_line, last_update, created_at, updated_at FROM( " \
    "            SELECT cin, corporate_body_name, street || ' ' || building_number || ', ' || postal_code || ' ' || city as address_line, updated_at last_update, NOW()::timestamp created_at, NOW()::timestamp updated_at, row_number() OVER( " \
    "                PARTITION BY cin " \
    "                ORDER BY updated_at DESC " \
    "            ) rw_number " \
    "            FROM ov.konkurz_vyrovnanie_issues WHERE cin IS NOT NULL " \
    "        ) query_3 " \
    "        WHERE rw_number = 1 " \
    "    ON CONFLICT (cin) DO NOTHING; " \
    " " \
    "    INSERT INTO ov.companies (cin, name, br_section, address_line, last_update, created_at, updated_at) " \
    "        SELECT cin, corporate_body_name, br_section, address_line, last_update, created_at, updated_at FROM( " \
    "            SELECT cin, corporate_body_name, br_section, street || ' ' || building_number || ', ' || postal_code || ' ' || city as address_line, updated_at last_update, NOW()::timestamp created_at, NOW()::timestamp updated_at, row_number() OVER( " \
    "                PARTITION BY cin " \
    "                ORDER BY updated_at DESC " \
    "            ) rw_number " \
    "            FROM ov.znizenie_imania_issues WHERE cin IS NOT NULL " \
    "        ) query_4 " \
    "        WHERE rw_number = 1 " \
    "    ON CONFLICT (cin) DO NOTHING; " \
    " " \
    "    INSERT INTO ov.companies (cin, name, address_line, last_update, created_at, updated_at) " \
    "        SELECT cin, corporate_body_name, address_line, last_update, created_at, updated_at FROM( " \
    "            SELECT cin, corporate_body_name, street || ' ' || building_number || ', ' || postal_code || ' ' || city as address_line, updated_at last_update, NOW()::timestamp created_at, NOW()::timestamp updated_at, row_number() OVER( " \
    "                PARTITION BY cin " \
    "                ORDER BY updated_at DESC " \
    "            ) rw_number " \
    "            FROM ov.konkurz_restrukturalizacia_actors WHERE cin IS NOT NULL " \
    "        ) query_5 " \
    "        WHERE rw_number = 1 " \
    "    ON CONFLICT (cin) DO NOTHING; " \
    " " \
    " " \
    "ALTER TABLE ov.or_podanie_issues  " \
    "	ADD COLUMN IF NOT EXISTS company_id bigint, " \
    "	ADD FOREIGN KEY (company_id) REFERENCES ov.companies(cin); " \
    " " \
    "UPDATE ov.or_podanie_issues " \
    "SET company_id = cin; " \
    " " \
    "ALTER TABLE ov.likvidator_issues  " \
    "	ADD COLUMN IF NOT EXISTS company_id bigint, " \
    "	ADD FOREIGN KEY (company_id) REFERENCES ov.companies(cin); " \
    " " \
    "UPDATE ov.likvidator_issues  " \
    "SET company_id = cin; " \
    " " \
    "ALTER TABLE ov.konkurz_vyrovnanie_issues " \
    "	ADD COLUMN IF NOT EXISTS company_id bigint, " \
    "	ADD FOREIGN KEY (company_id) REFERENCES ov.companies(cin); " \
    " " \
    "UPDATE ov.konkurz_vyrovnanie_issues " \
    "SET company_id = cin; " \
    " " \
    "ALTER TABLE ov.znizenie_imania_issues  " \
    "	ADD COLUMN IF NOT EXISTS company_id bigint, " \
    "	ADD FOREIGN KEY (company_id) REFERENCES ov.companies(cin); " \
    " " \
    "UPDATE ov.znizenie_imania_issues " \
    "SET company_id = cin; " \
    " " \
    "ALTER TABLE ov.konkurz_restrukturalizacia_actors  " \
    "	ADD COLUMN IF NOT EXISTS company_id bigint, " \
    "	ADD FOREIGN KEY (company_id) REFERENCES ov.companies(cin); " \
    " " \
    "UPDATE ov.konkurz_restrukturalizacia_actors " \
    "SET company_id = cin;"

    dependencies = [
    ]

    operations = [
        migrations.RunSQL(prikaz)
    ]