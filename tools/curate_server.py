# curate_server.py  —  Private local server for the model organizer.
# Serves the repo (so GLB previews load) and saves your arrangement to
# library/curation.json. Nothing here is published; it runs on your machine only.

import http.server, socketserver, os, sys, webbrowser, threading

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
PORT = 8770
os.chdir(ROOT)

class Handler(http.server.SimpleHTTPRequestHandler):
    def do_POST(self):
        if self.path == '/save':
            n = int(self.headers.get('Content-Length', 0))
            data = self.rfile.read(n)
            os.makedirs(os.path.join(ROOT, 'library'), exist_ok=True)
            with open(os.path.join(ROOT, 'library', 'curation.json'), 'wb') as f:
                f.write(data)
            self.send_response(200)
            self.send_header('Content-Type', 'application/json')
            self.end_headers()
            self.wfile.write(b'{"ok":true}')
        else:
            self.send_error(404)

    def end_headers(self):
        self.send_header('Cache-Control', 'no-store')
        super().end_headers()

    def log_message(self, *a):
        pass

url = f"http://127.0.0.1:{PORT}/curate.html"
threading.Timer(1.0, lambda: webbrowser.open(url)).start()
print("=" * 56)
print("  MODEL ORGANIZER  ->  " + url)
print("  Leave this window open while you sort. Close it when done.")
print("=" * 56)
with socketserver.TCPServer(("127.0.0.1", PORT), Handler) as httpd:
    try:
        httpd.serve_forever()
    except KeyboardInterrupt:
        pass
