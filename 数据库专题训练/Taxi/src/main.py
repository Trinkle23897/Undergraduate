import web, json, taxi

urls = (
    "/", "Home",
    "/pos", "Pos",
    "/query", "Query",
    "/about", "About"
)

def check(obj):
    results = obj["result"]
    assert 6 <= len(results) <= 11
    for result in results[6:]:
        assert len(result) == 6
        for i in range(5):
            assert isinstance(result[i], int)
        car_id, n_place, dis, detour0, detour1, routes = result
        assert car_id <= 100000 and dis <= 10000 and detour0 <= 10000 and detour1 <= 10000 and len(routes) == n_place - 1
        for i in range(len(routes) - 1):
            assert routes[i][-1][0] == routes[i + 1][0][0]

class Home:
    def GET(self):
        raise web.seeother("/static/home.html")

class About:
    def GET(self):
        raise web.seeother("/static/about.html")

class Pos:
    def POST(self):
        data = json.loads(web.data().decode())
        return taxi.get_pos(data["lng"], data["lat"]).decode()

class Query:
    def POST(self):
        data = json.loads(web.data().decode())
        res = taxi.query(data["origin"], data["dest"]).decode()
        check(json.loads(res))
        return res

if __name__ == "__main__":
    taxi.init()
    web.application(urls, globals()).run()
