from django.http import HttpResponse

import json
import collections
import datetime
import numbers
import time


from django.db import connection as pripojenie
from django.views.decorators.csrf import csrf_exempt

from . import models
from django.db.models import Q
from django.utils import timezone

def v1companies(request):
    page = request.GET.get("page")
    per_page = request.GET.get("per_page")
    vyhladavanie = request.GET.get("query")
    gte_vyhladavanie = request.GET.get("last_update_gte")
    lte_vyhladavanie = request.GET.get("last_update_lte")
    order = request.GET.get("order_by")
    order_typ = request.GET.get("order_type")

    if(page == None or int(page) <= 0):
        page = 1
    else:
        page = int(page)
    if(per_page == None or int(per_page) <= 0):
        per_page = 10
    else:
        per_page = int(per_page)

    #vyhladavanie nad stlpcami
    if(vyhladavanie != None):
        where_pomocna = 1
        prikaz_hladanie = "WHERE name LIKE '%%{}%%' OR address_line LIKE '%%{}%%'".format(vyhladavanie, vyhladavanie)
    else:
        prikaz_hladanie = ""


     #date vyhladavanie
    if(gte_vyhladavanie != None):
        try:
            datetime.datetime.fromisoformat(gte_vyhladavanie)
        except ValueError:
            gte_vyhladavanie = None

    if(lte_vyhladavanie != None):
        try:
            datetime.datetime.fromisoformat(lte_vyhladavanie)
        except ValueError:
            lte_vyhladavanie = None
        
    #gte
    if(gte_vyhladavanie != None):
        if(where_pomocna == 0):
            where_pomocna = 1
            prikaz_date_hladanie_gte = "WHERE last_update >= date('{}')".format(gte_vyhladavanie)
        else:
            prikaz_date_hladanie_gte = "AND last_update >= date('{}')".format(gte_vyhladavanie)
    else:
        prikaz_date_hladanie_gte = ""

    #lte
    if(lte_vyhladavanie != None):
        if(where_pomocna == 0):
            where_pomocna = 1
            prikaz_date_hladanie_lte = "WHERE last_update <= date('{}')".format(lte_vyhladavanie)
        else:
            prikaz_date_hladanie_lte = "AND last_update <= date('{}')".format(lte_vyhladavanie)
    else:
        prikaz_date_hladanie_lte = ""

    
    #stlpce
    stlpce = "cin, name, br_section, address_line, last_update, or_podanie_issues_count, znizenie_imania_issues_count, likvidator_issues_count, konkurz_vyrovnanie_issues_count, konkurz_restrukturalizacia_actors_count"

    #order
    if(order_typ == "asc"):
        order_typ_prikaz = order_typ
    else:
        order_typ_prikaz = "desc"

    if(order != None):
        #kontrola, ci take existuje
        if order in stlpce and not "," in order and not " " in order:
            order_prikaz = "ORDER BY {} {}".format(order, order_typ_prikaz)
        else:
            order_prikaz = "ORDER BY cin {}".format(order_typ_prikaz)
    else:
        order_prikaz = "ORDER BY cin {}".format(order_typ_prikaz)


    #prikaz = "SELECT COUNT(*) FROM ov.or_podanie_issues {} {} {}".format(prikaz_hladanie, prikaz_date_hladanie_gte, prikaz_date_hladanie_lte)

    cur = pripojenie.cursor()
    prikaz = "SELECT COUNT(*) FROM ov.companies {} {} {}".format(prikaz_hladanie, prikaz_date_hladanie_gte, prikaz_date_hladanie_lte)
    cur.execute(prikaz)
    meta_count = cur.fetchall()[0][0]


    cur.execute("SELECT main.cin, main.name, main.br_section, main.address_line, main.last_update, s1.or_podanie_issues_count, s2.znizenie_imania_issues_count, s3.likvidator_issues_count, s4.konkurz_vyrovnanie_issues_count, s5.konkurz_restrukturalizacia_actors_count "\
    "FROM ov.companies main "\
    "LEFT JOIN "\
    "	(SELECT cin, count(cin) as or_podanie_issues_count FROM ov.or_podanie_issues GROUP BY cin) s1 ON s1.cin = main.cin "\
    "LEFT JOIN "\
    "	(SELECT cin, count(cin) as znizenie_imania_issues_count FROM ov.znizenie_imania_issues GROUP BY cin) s2 ON s2.cin = main.cin "\
    "LEFT JOIN "\
    "	(SELECT cin, count(cin) as likvidator_issues_count FROM ov.likvidator_issues GROUP BY cin) s3 ON s3.cin = main.cin "\
    "LEFT JOIN "\
    "	(SELECT cin, count(cin) as konkurz_vyrovnanie_issues_count FROM ov.konkurz_vyrovnanie_issues GROUP BY cin) s4 ON s4.cin = main.cin "\
    "LEFT JOIN "\
    "	(SELECT cin, count(cin) as konkurz_restrukturalizacia_actors_count FROM ov.konkurz_restrukturalizacia_actors GROUP BY cin) s5 ON s5.cin = main.cin "\
    "{} {} {} {} LIMIT {} OFFSET {}".format(prikaz_hladanie, prikaz_date_hladanie_gte, prikaz_date_hladanie_lte, order_prikaz, per_page, (page - 1) * per_page))
    riadky = cur.fetchall()
    cur.close()


    #formatovanie
    objects_list = []
    hlavny = collections.OrderedDict()
    for row in riadky:
        dictionary = collections.OrderedDict()
        dictionary["cin"] = int(row[0]) if row[0] != None else None
        dictionary["name"] = row[1]
        dictionary["br_section"] = row[2]
        dictionary["address_line"] = row[3]
        dictionary["last_update"] = str(row[4])
        dictionary["or_podanie_issues_count"] = int(row[5]) if row[5] != None else None
        dictionary["znizenie_imania_issues_count"] = int(row[6]) if row[6] != None else None
        dictionary["likvidator_issues_count"] = int(row[7]) if row[7] != None else None
        dictionary["konkurz_vyrovnanie_issues_count"] = int(row[8]) if row[8] != None else None
        dictionary["konkurz_restrukturalizacia_actors_count"] = int(row[9]) if row[9] != None else None
        objects_list.append(dictionary)
    hlavny["items"] = objects_list

    #metadata
    dictionary = collections.OrderedDict()
    dictionary["page"] = page
    dictionary["per_page"] = per_page
    dictionary["pages"] = int(meta_count / per_page + 1)
    dictionary["total"] = meta_count
    hlavny["metadata"] = dictionary
        
    vysledok = json.dumps(hlavny, indent = 1, ensure_ascii = False).encode('utf8')

    global response_status
    response_status = 200

    return HttpResponse(vysledok.decode(), content_type="application/json")

def v1Health(request):

    cur = pripojenie.cursor()
    cur.execute("SELECT date_trunc('second', current_timestamp - pg_postmaster_start_time()) as uptime")
    vysledok = cur.fetchall()
    cur.close()
    pripojenie.close()

    vysledok = str(vysledok[0][0]).replace(",", "")

    return HttpResponse('{ "pgsql": { "uptime": "' + vysledok + '" } }', content_type="application/json")

response_status = 300

@csrf_exempt
def v1Submissions(request):
    if request.method == "GET":
        return HttpResponse(v1Submissions_get(request), content_type="application/json", status = response_status)
    elif request.method == "POST":
        return HttpResponse(v1Submissions_post(request), content_type="application/json", status = response_status)
    return 0

@csrf_exempt
def v1Submissions_delete(request, delete_id):
    global response_status

    #ak sa nejedna o delete
    if request.method != "DELETE":
        response_status = 404
        return  HttpResponse("", content_type="application/json", status = response_status)


    #zistit, ci existuje
    cur = pripojenie.cursor()
    cur.execute("SELECT id, bulletin_issue_id, raw_issue_id FROM ov.or_podanie_issues WHERE id = {}".format(delete_id))
    vysledok = cur.fetchall()
    
    #ak existuje
    if vysledok:
        bullet_id = vysledok[0][1]
        raw_id = vysledok[0][2]
        response_status = 204
        vysledok = ""

        #vymazanie z hlavnej tabulky
        cur.execute("DELETE FROM ov.or_podanie_issues WHERE id = {} RETURNING *".format(delete_id))
        vysledok_main = cur.fetchall()
        if(vysledok_main):
            print("Deleted from main")
            print(vysledok_main)
        else:
            print("Record was not found in main")
        
        #vymazanie z tabulky raw
        cur.execute("SELECT id FROM ov.or_podanie_issues WHERE id != {} AND raw_issue_id = {}".format(delete_id, raw_id))
        vysledok_raw = cur.fetchall()
        if not vysledok_raw:
            cur.execute("DELETE FROM ov.raw_issues WHERE id = {} RETURNING *".format(raw_id))
            vysledok_raw = cur.fetchall()
            if(vysledok_raw):
                print("Deleted from raw")
                print(vysledok_raw)
            else:
                print("Record was not found in raw")
            
        #vymazanie z tabulky bullet
        cur.execute("SELECT id FROM ov.or_podanie_issues WHERE id != {} AND bulletin_issue_id = {}".format(delete_id, bullet_id))
        vysledok_bullet = cur.fetchall()
        if not vysledok_bullet:
            cur.execute("DELETE FROM ov.bulletin_issues WHERE id = {} RETURNING *".format(bullet_id))
            vysledok_bullet = cur.fetchall()
            if vysledok_bullet:
                print("Deleted from bullet")
                print(vysledok_bullet)
            else:
                print("Record was not found in bullet")
        
    else:
        response_status = 404
        response_dictionary = collections.OrderedDict()
        response_dictionary_vnutro = collections.OrderedDict()
        response_dictionary_vnutro["message"] = "Záznam neexistuje"
        response_dictionary["error"] = response_dictionary_vnutro
        vysledok = json.dumps(response_dictionary, indent = 1, ensure_ascii = False).encode('utf8').decode()




    cur.close()
    pripojenie.close()
    
    return HttpResponse(vysledok, content_type="application/json", status = response_status)
    
def v1Submissions_post(request):
    #ziskam data z POST do dictionary
    post_data = json.loads(request.body.decode("utf-8"))

    #validacia, povinne polia
    povinne_polia = ["br_court_name", "kind_name", "cin", "registration_date", "corporate_body_name", "br_section", "br_insertion", "text", "street", "postal_code", "city"]
    errors_vypis = collections.OrderedDict()
    objects_list = []
    for i in povinne_polia:
        dictionary = collections.OrderedDict()
        if not i in post_data:
            dictionary["field"] = i
            dictionary["reasons"] = ["required"]
        #specialne polia
        elif "cin" in i and not isinstance(post_data["cin"], numbers.Number):
            dictionary["field"] = i
            dictionary["reasons"] = ["required", "not_number"]
        elif "registration_date" in i:
            try:
                datetime.date.fromisoformat(post_data["registration_date"])
            except ValueError:
                dictionary["field"] = i
                dictionary["reasons"] = ["required", "wrong_format"]
            if not dictionary and datetime.date.today().year != int(post_data["registration_date"].split('-')[0]):
                dictionary["field"] = i
                dictionary["reasons"] = ["required", "invalid_range"]
        elif type(post_data[i]) != str and not "cin" in i:
            dictionary["field"] = i
            dictionary["reasons"] = ["required", "must_be_string"]


        #ak v nom nieco je, tak ho pridam
        if dictionary:
            objects_list.append(dictionary)

    errors_vypis["errors"] = objects_list

    global response_status
    if objects_list:
        response_status = 422
        vysledok = json.dumps(errors_vypis, indent = 1, ensure_ascii = False).encode('utf8')
    else: #ak nebude error, tak vypisem, co zapisem
        response_status = 201

        cur = pripojenie.cursor()
        cur.execute("SELECT number FROM ov.bulletin_issues ORDER BY number desc LIMIT 1")
        last_number = cur.fetchall()[0][0]
        cur.execute("INSERT INTO ov.bulletin_issues (year, number, published_at, created_at, updated_at) VALUES ({}, {}, '{}', '{}', '{}') RETURNING id".format(datetime.date.today().year, last_number + 1, datetime.datetime.today() , datetime.datetime.today(), datetime.datetime.today()))
        bullet_id = cur.fetchall()[0][0]
        cur.execute("INSERT INTO ov.raw_issues (bulletin_issue_id, file_name, content, created_at, updated_at) VALUES ({}, '{}', '{}', '{}', '{}') RETURNING id".format(bullet_id, '-', '-', datetime.datetime.today(), datetime.datetime.today()))
        raw_id = cur.fetchall()[0][0]
        cur.execute("INSERT INTO ov.or_podanie_issues (bulletin_issue_id, raw_issue_id, br_mark, br_court_code, br_court_name, kind_code, kind_name, cin, registration_date, corporate_body_name, br_section, br_insertion, text, created_at, updated_at, address_line, street, postal_code, city) VALUES ({}, {}, '{}', '{}', '{}', '{}', '{}', {}, '{}', '{}', '{}', '{}', '{}', '{}', '{}', '{}', '{}', '{}', '{}') RETURNING id".format(bullet_id, raw_id, '-', '-', post_data["br_court_name"], '-', post_data["kind_name"], post_data["cin"], post_data["registration_date"], post_data["corporate_body_name"], post_data["br_section"], post_data["br_insertion"], post_data["text"], datetime.datetime.today(), datetime.datetime.today(), post_data["street"] + ", " + post_data["postal_code"] + " " + post_data["city"], post_data["street"], post_data["postal_code"], post_data["city"]))
        main_id = cur.fetchall()[0][0]
        cur.close()
        pripojenie.close()

        
        response_dictionary = collections.OrderedDict()
        response_dictionary["response"] = collections.OrderedDict()
        response_dictionary["response"]["id"] = main_id
        response_dictionary["response"]["br_court_name"] = post_data["br_court_name"]
        response_dictionary["response"]["kind_name"] = post_data["kind_name"]
        response_dictionary["response"]["cin"] = post_data["cin"]
        response_dictionary["response"]["registration_date"] = post_data["registration_date"]
        response_dictionary["response"]["corporate_body_name"] = post_data["corporate_body_name"]
        response_dictionary["response"]["br_section"] = post_data["br_section"]
        response_dictionary["response"]["br_insertion"] = post_data["br_insertion"]
        response_dictionary["response"]["text"] = post_data["text"]
        response_dictionary["response"]["street"] = post_data["street"]
        response_dictionary["response"]["postal_code"] = post_data["postal_code"]
        response_dictionary["response"]["city"] = post_data["city"]

        vysledok = json.dumps(response_dictionary, indent = 1, ensure_ascii = False).encode('utf8')
        
    
    return vysledok.decode()

def v1Submissions_get(request):
    page = request.GET.get("page")
    per_page = request.GET.get("per_page")
    vyhladavanie = request.GET.get("query")
    gte_vyhladavanie = request.GET.get("registration_date_gte")
    lte_vyhladavanie = request.GET.get("registration_date_lte")
    order = request.GET.get("order_by")
    order_typ = request.GET.get("order_type")

    if(page == None or int(page) <= 0):
        page = 1
    else:
        page = int(page)
    if(per_page == None or int(per_page) <= 0):
        per_page = 10
    else:
        per_page = int(per_page)
    
    

    cur = pripojenie.cursor()

    where_pomocna = 0
    #vyhladavanie nad stlpcami
    if(vyhladavanie != None):
        where_pomocna = 1
        if(vyhladavanie.isdigit()):
            prikaz_hladanie = "WHERE corporate_body_name LIKE '%%{}%%' OR cin = {} OR city LIKE '%%{}%%'".format(vyhladavanie, int(vyhladavanie), vyhladavanie)
        else:
            prikaz_hladanie = "WHERE corporate_body_name LIKE '%%{}%%' OR city LIKE '%%{}%%'".format(vyhladavanie, vyhladavanie)
    else:
        prikaz_hladanie = ""

    
    #date vyhladavanie
    if(gte_vyhladavanie != None):
        try:
            datetime.datetime.fromisoformat(gte_vyhladavanie)
        except ValueError:
            gte_vyhladavanie = None

    if(lte_vyhladavanie != None):
        try:
            datetime.datetime.fromisoformat(lte_vyhladavanie)
        except ValueError:
            lte_vyhladavanie = None
        
    

    #gte
    if(gte_vyhladavanie != None):
        if(where_pomocna == 0):
            where_pomocna = 1
            prikaz_date_hladanie_gte = "WHERE registration_date >= date('{}')".format(gte_vyhladavanie)
        else:
            prikaz_date_hladanie_gte = "AND registration_date >= date('{}')".format(gte_vyhladavanie)
    else:
        prikaz_date_hladanie_gte = ""

    #lte
    if(lte_vyhladavanie != None):
        if(where_pomocna == 0):
            where_pomocna = 1
            prikaz_date_hladanie_lte = "WHERE registration_date <= date('{}')".format(lte_vyhladavanie)
        else:
            prikaz_date_hladanie_lte = "AND registration_date <= date('{}')".format(lte_vyhladavanie)
    else:
        prikaz_date_hladanie_lte = ""

    #stlpce
    stlpce = "id, br_court_name, kind_name, cin, registration_date, corporate_body_name, br_section, br_insertion, text, street, postal_code, city"

    #order
    if(order_typ == "asc"):
        order_typ_prikaz = order_typ
    else:
        order_typ_prikaz = "desc"

    if(order != None):
        #kontrola, ci take existuje
        if order in stlpce and not "," in order and not " " in order:
            order_prikaz = "ORDER BY {} {}".format(order, order_typ_prikaz)
        else:
            order_prikaz = "ORDER BY id {}".format(order_typ_prikaz)
    else:
        order_prikaz = "ORDER BY id {}".format(order_typ_prikaz)


    
    prikaz = "SELECT COUNT(*) FROM ov.or_podanie_issues {} {} {}".format(prikaz_hladanie, prikaz_date_hladanie_gte, prikaz_date_hladanie_lte)
    cur.execute(prikaz)
    meta_count = cur.fetchall()[0][0]
    prikaz = "SELECT {} FROM ov.or_podanie_issues {} {} {} {} LIMIT {} OFFSET {}".format(stlpce, prikaz_hladanie, prikaz_date_hladanie_gte, prikaz_date_hladanie_lte, order_prikaz, per_page, (page - 1) * per_page)
    cur.execute(prikaz)
    riadky = cur.fetchall()
    
    

    #formatovanie
    objects_list = []
    hlavny = collections.OrderedDict()
    for row in riadky:
        dictionary = collections.OrderedDict()
        dictionary["id"] = row[0]
        dictionary["br_court_name"] = row[1]
        dictionary["kind_name"] = row[2]
        dictionary["cin"] = row[3]
        dictionary["registration_date"] = str(row[4])
        dictionary["corporate_body_name"] = row[5]
        dictionary["br_section"] = row[6]
        dictionary["br_insertion"] = row[7]
        dictionary["text"] = row[8]
        dictionary["street"] = row[9]
        dictionary["postal_code"] = row[10]
        dictionary["city"] = row[11]
        objects_list.append(dictionary)
    hlavny["items"] = objects_list

    #metadata
    dictionary = collections.OrderedDict()
    dictionary["page"] = page
    dictionary["per_page"] = per_page
    dictionary["pages"] = int(meta_count / per_page + 1)
    dictionary["total"] = meta_count
    hlavny["metadata"] = dictionary
        
    vysledok = json.dumps(hlavny, indent = 1, ensure_ascii = False).encode('utf8')


    cur.close()
    pripojenie.close()

    global response_status
    response_status = 200
    return vysledok.decode().replace("\\n", " ")


@csrf_exempt
def v2Submissions(request):
    if request.method == "GET":
        return HttpResponse(v2Submissions_get(request), content_type="application/json", status = response_status)
    elif request.method == "POST":
        return HttpResponse(v2Submissions_post(request), content_type="application/json", status = response_status)
    return 0

def v2Submissions_get(request):
    page = request.GET.get("page")
    per_page = request.GET.get("per_page")
    vyhladavanie = request.GET.get("query")
    gte_vyhladavanie = request.GET.get("registration_date_gte")
    lte_vyhladavanie = request.GET.get("registration_date_lte")
    order = request.GET.get("order_by")
    order_typ = request.GET.get("order_type")

    if(page == None or int(page) <= 0):
        page = 1
    else:
        page = int(page)
    if(per_page == None or int(per_page) <= 0):
        per_page = 10
    else:
        per_page = int(per_page)
    
    
    table = models.OrPodanieIssues.objects.only("id", "br_court_name", "kind_name", "cin", "registration_date", "corporate_body_name", "br_section", "br_insertion", "text", "street", "postal_code", "city")

    #vyhladavanie nad stlpcami
    if(vyhladavanie != None):
        if(vyhladavanie.isdigit()):
            table = table.filter(Q(corporate_body_name__icontains=vyhladavanie) or Q(cin=int(vyhladavanie)) or Q(city__icontains=vyhladavanie))
        else:
            table = table.filter(Q(corporate_body_name__icontains=vyhladavanie) or Q(city__icontains=vyhladavanie))

    
    #date vyhladavanie
    if(gte_vyhladavanie != None):
        try:
            datetime.datetime.fromisoformat(gte_vyhladavanie)
        except ValueError:
            gte_vyhladavanie = None

    if(lte_vyhladavanie != None):
        try:
            datetime.datetime.fromisoformat(lte_vyhladavanie)
        except ValueError:
            lte_vyhladavanie = None
        
    
    #gte
    if(gte_vyhladavanie != None):
        table = table.filter(registration_date__gte=datetime.datetime.fromisoformat(gte_vyhladavanie))

    #lte
    if(lte_vyhladavanie != None):
        table = table.filter(registration_date__lte=datetime.datetime.fromisoformat(lte_vyhladavanie))

    #stlpce
    stlpce = "id, br_court_name, kind_name, cin, registration_date, corporate_body_name, br_section, br_insertion, text, street, postal_code, city"

    #order
    if(order_typ == "asc"):
        order_typ_prikaz = ""
    else:
        order_typ_prikaz = "-"

    if(order != None):
        #kontrola, ci take existuje
        if order in stlpce and not "," in order and not " " in order:
            table = table.order_by(order_typ_prikaz + order)
        else:
            table = table.order_by(order_typ_prikaz + "id")
    else:
        table = table.order_by(order_typ_prikaz + "id")

    meta_count = table.count()
    table = table.all()[(page - 1)*per_page:(page)*per_page]

    #formatovanie
    objects_list = []
    hlavny = collections.OrderedDict()
    for row in table:
        dictionary = collections.OrderedDict()
        dictionary["id"] = row.id
        dictionary["br_court_name"] = row.br_court_name
        dictionary["kind_name"] = row.kind_name
        dictionary["cin"] = row.cin
        dictionary["registration_date"] = str(row.registration_date)
        dictionary["corporate_body_name"] = row.corporate_body_name
        dictionary["br_section"] = row.br_section
        dictionary["br_insertion"] = row.br_insertion
        dictionary["text"] = row.text
        dictionary["street"] = row.street
        dictionary["postal_code"] = row.postal_code
        dictionary["city"] = row.city
        objects_list.append(dictionary)
    hlavny["items"] = objects_list

    #metadata
    dictionary = collections.OrderedDict()
    dictionary["page"] = page
    dictionary["per_page"] = per_page
    dictionary["pages"] = int(meta_count / per_page + 1)
    dictionary["total"] = meta_count
    hlavny["metadata"] = dictionary
        
    vysledok = json.dumps(hlavny, indent = 1, ensure_ascii = False).encode('utf8')


    global response_status
    response_status = 200
    return vysledok.decode().replace("\\n", " ")

@csrf_exempt
def v2Submissions_post(request):
    #ziskam data z POST do dictionary
    post_data = json.loads(request.body.decode("utf-8"))

    #validacia, povinne polia
    povinne_polia = ["br_court_name", "kind_name", "cin", "registration_date", "corporate_body_name", "br_section", "br_insertion", "text", "street", "postal_code", "city"]
    errors_vypis = collections.OrderedDict()
    objects_list = []
    for i in povinne_polia:
        dictionary = collections.OrderedDict()
        if not i in post_data:
            dictionary["field"] = i
            dictionary["reasons"] = ["required"]
        #specialne polia
        elif "cin" in i and not isinstance(post_data["cin"], numbers.Number):
            dictionary["field"] = i
            dictionary["reasons"] = ["required", "not_number"]
        elif "registration_date" in i:
            try:
                datetime.date.fromisoformat(post_data["registration_date"])
            except ValueError:
                dictionary["field"] = i
                dictionary["reasons"] = ["required", "wrong_format"]
            if not dictionary and datetime.date.today().year != int(post_data["registration_date"].split('-')[0]):
                dictionary["field"] = i
                dictionary["reasons"] = ["required", "invalid_range"]
        elif type(post_data[i]) != str and not "cin" in i:
            dictionary["field"] = i
            dictionary["reasons"] = ["required", "must_be_string"]


        #ak v nom nieco je, tak ho pridam
        if dictionary:
            objects_list.append(dictionary)

    errors_vypis["errors"] = objects_list

    global response_status
    if objects_list:
        response_status = 422
        vysledok = json.dumps(errors_vypis, indent = 1, ensure_ascii = False).encode('utf8')
    else: #ak nebude error, tak vypisem, co zapisem
        response_status = 201

        last_number = models.BulletinIssues.objects.only("number")
        last_number = last_number.order_by("-number")[:1][0].number + 1
        bullet_id = models.BulletinIssues.objects.create(year = datetime.date.today().year, number = last_number, published_at = timezone.now(), created_at = timezone.now(), updated_at = timezone.now()).id
        raw_id = models.RawIssues.objects.create(bulletin_issue_id = bullet_id, file_name = '-', content = '-', created_at = timezone.now(), updated_at = timezone.now()).id
        main_id = models.OrPodanieIssues.objects.create(bulletin_issue_id = bullet_id, raw_issue_id = raw_id, br_mark = '-', br_court_code = '-', br_court_name = post_data["br_court_name"], kind_code = '-', kind_name = post_data["kind_name"], cin = post_data["cin"], registration_date = post_data["registration_date"], corporate_body_name = post_data["corporate_body_name"], br_section = post_data["br_section"], br_insertion = post_data["br_insertion"], text = post_data["text"], created_at = timezone.now(), updated_at = timezone.now(), address_line = post_data["street"] + ", " + post_data["postal_code"] + " " + post_data["city"], street = post_data["street"], postal_code = post_data["postal_code"], city = post_data["city"]).id
        
        response_dictionary = collections.OrderedDict()
        response_dictionary["response"] = collections.OrderedDict()
        response_dictionary["response"]["id"] = main_id
        response_dictionary["response"]["br_court_name"] = post_data["br_court_name"]
        response_dictionary["response"]["kind_name"] = post_data["kind_name"]
        response_dictionary["response"]["cin"] = post_data["cin"]
        response_dictionary["response"]["registration_date"] = post_data["registration_date"]
        response_dictionary["response"]["corporate_body_name"] = post_data["corporate_body_name"]
        response_dictionary["response"]["br_section"] = post_data["br_section"]
        response_dictionary["response"]["br_insertion"] = post_data["br_insertion"]
        response_dictionary["response"]["text"] = post_data["text"]
        response_dictionary["response"]["street"] = post_data["street"]
        response_dictionary["response"]["postal_code"] = post_data["postal_code"]
        response_dictionary["response"]["city"] = post_data["city"]

        vysledok = json.dumps(response_dictionary, indent = 1, ensure_ascii = False).encode('utf8')
    
    return vysledok.decode()

@csrf_exempt
def v2Submissions_int(request, number):
    if request.method == "GET":
        return v2Submissions_int_get(request, number)
    elif request.method == "DELETE":
        return v2Submissions_int_delete(request, number)
    else:
        response_status = 404
        return  HttpResponse("", content_type="application/json", status = response_status)


@csrf_exempt
def v2Submissions_int_delete(request, delete_id):
    global response_status


    #zistit, ci existuje
    vysledok = models.OrPodanieIssues.objects.only("id", "bulletin_issue_id", "raw_issue_id")
    vysledok2 = vysledok.filter(id=delete_id)
    
    
    #ak existuje
    if vysledok2:
        bullet_id = vysledok2[0].bulletin_issue_id
        raw_id = vysledok2[0].raw_issue_id
        response_status = 204

        vysledok2.delete()
        print("Deleted from main")


        vysledok_raw = vysledok.filter(raw_issue_id=raw_id).exclude(id=delete_id)

        if not vysledok_raw:
            vysledok_raw = models.RawIssues.objects.only("id")
            vysledok_raw = vysledok_raw.filter(id=raw_id)
            vysledok_raw.delete()
            print("Deleted from raw")


        vysledok_bullet = vysledok.filter(bulletin_issue_id=bullet_id).exclude(id=bullet_id)
        if not vysledok_bullet:
            vysledok_bullet = models.BulletinIssues.objects.only("id")
            vysledok_bullet = vysledok_bullet.filter(id=bullet_id)
            vysledok_bullet.delete()
            print("Deleted from bullet")

        vysledok = ""
        
    else:
        response_status = 404
        response_dictionary = collections.OrderedDict()
        response_dictionary_vnutro = collections.OrderedDict()
        response_dictionary_vnutro["message"] = "Záznam neexistuje"
        response_dictionary["error"] = response_dictionary_vnutro
        vysledok = json.dumps(response_dictionary, indent = 1, ensure_ascii = False).encode('utf8').decode()

    
    return HttpResponse(vysledok, content_type="application/json", status = response_status)


def v2Submissions_int_get(request, number):
    global response_status

    vysledok = models.OrPodanieIssues.objects.only("id", "br_court_name", "kind_name", "cin", "registration_date", "corporate_body_name", "br_section", "br_insertion", "text", "street", "postal_code", "city")
    vysledok = vysledok.filter(id=number)

    if vysledok:
        vysledok = vysledok[0]
        response_status = 200
        response_dictionary = collections.OrderedDict()
        response_dictionary["response"] = collections.OrderedDict()
        response_dictionary["response"]["id"] = vysledok.id
        response_dictionary["response"]["br_court_name"] = vysledok.br_court_name
        response_dictionary["response"]["kind_name"] = vysledok.kind_name
        response_dictionary["response"]["cin"] = vysledok.cin
        response_dictionary["response"]["registration_date"] = str(vysledok.registration_date)
        response_dictionary["response"]["corporate_body_name"] = vysledok.corporate_body_name
        response_dictionary["response"]["br_section"] = vysledok.br_section
        response_dictionary["response"]["br_insertion"] = vysledok.br_insertion
        response_dictionary["response"]["text"] = vysledok.text
        response_dictionary["response"]["street"] = vysledok.street
        response_dictionary["response"]["postal_code"] = vysledok.postal_code
        response_dictionary["response"]["city"] = vysledok.city

        vysledok = json.dumps(response_dictionary, indent = 1, ensure_ascii = False).encode('utf8')
    else:
        response_status = 404
        response_dictionary = collections.OrderedDict()
        response_dictionary_vnutro = collections.OrderedDict()
        response_dictionary_vnutro["message"] = "Záznam neexistuje"
        response_dictionary["error"] = response_dictionary_vnutro
        vysledok = json.dumps(response_dictionary, indent = 1, ensure_ascii = False).encode('utf8').decode()

    return HttpResponse(vysledok, content_type="application/json", status = response_status)