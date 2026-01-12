// Readability is injected globally by Readability.js
declare const Readability: any;

function extractJS(): string {
    try {
        const article = new Readability(document).parse();
        if (!article) return "";

        return `
<!DOCTYPE html>
<html>
<head>
<meta charset="utf-8">
<style>
body {
    font-family: system-ui, -apple-system, BlinkMacSystemFont, sans-serif;
    line-height: 1.6;
    font-size: 18px;
    margin: 24px;
    color: #222;
    background: white;
}
h1, h2, h3 {
    line-height: 1.3;
}
img {
    max-width: 100%;
    height: auto;
}
pre {
    background: #f4f4f4;
    padding: 12px;
    overflow-x: auto;
}
</style>
</head>
<body>
<h1>${article.title || ""}</h1>
${article.content}
</body>
</html>`;
    } catch (e) {
        return "";
    }
}

// Main (IIFE)
((): string => {
    return extractJS();
})();
