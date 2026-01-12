#ifndef ARTICLE_EXTRACTOR_CLASS
#define ARTICLE_EXTRACTOR_CLASS

#include <QObject>
#include <QWebEnginePage>
#include <QWebEngineProfile>
#include <QFile>
#include <QUrl>
#include <QDebug>

class ArticleExtractor : public QObject {
    Q_OBJECT

public:
    explicit ArticleExtractor(QObject* parent = nullptr)
        : QObject(parent)
        , page(new QWebEnginePage(QWebEngineProfile::defaultProfile(), this))
    {}

    void extract(const QUrl& url)
    {
        // Prevent duplicate signal connections
        disconnect(page, nullptr, this, nullptr);

        connect(page, &QWebEnginePage::loadFinished,
                this, &ArticleExtractor::onLoadFinished);

        page->load(url);
    }

signals:
    void finished(const QString& html);   // <-- now HTML
    void failed(const QString& reason);

private:
    QWebEnginePage* page = nullptr;

    static QString loadResource(const QString& path)
    {
        QFile f(path);
        if (!f.open(QIODevice::ReadOnly))
            return {};
        return QString::fromUtf8(f.readAll());
    }

private slots:
    void onLoadFinished(bool ok)
    {
        if (!ok) {
            emit failed("Page load failed");
            return;
        }

        Q_ASSERT(QFile(":/js/Readability.js").exists());
        Q_ASSERT(QFile(":/js/extract_article.js").exists());


        // 1️⃣ Inject Readability.js
        const QString readabilityJs = loadResource(":/js/Readability.js");


        page->runJavaScript(readabilityJs, [this](const QVariant&) {
            const QString extractJs = loadResource(":/js/extract_article.js");
            // 2️⃣ Extract formatted HTML
            page->runJavaScript(extractJs, [this](const QVariant& result) {

                const QString html = result.toString().trimmed();
                if (html.isEmpty())
                    emit failed("No article content found");
                else
                    emit finished(html);
            });
        });
    }
};

#endif // ARTICLE_EXTRACTOR_CLASS
