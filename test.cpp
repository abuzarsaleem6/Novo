#include "Comment.h"
#include "PostWidget.h"
#include "SearchEngine.h"
#include "User.h"
#include <QDebug>

int main() {
    // test code yahan
    // 1. Post banao
    Posts p("Hello world!", "ali");

    // 2. Comments add karo
    p.addComment("Nice post!", "sara");
    p.addComment("Great!", "ali");

    // 3. Comments check karo
    QList<Comment> comments = p.getComments();
    qDebug() << "Total comments:" << comments.size();

    for (const Comment& c : comments) {
        qDebug() << c.getCreatorUsername() << ":" << c.getContent();
    }

    // 4. Edit test karo
    p.editComment(0, "Updated comment!", "sara");
    qDebug() << "After edit:" << p.getComments()[0].getContent();

    // 5. Delete test karo
    p.deleteComment(0, "sara");
    qDebug() << "After delete, total:" << p.getComments().size();

    // 6. Save test karo
    p.saveCommentsToFile();
    qDebug() << "Saved!";

    // 7. Load test karo
    Posts p2("Hello world!", "ali");
    p2.loadCommentsFromFile();
    qDebug() << "Loaded comments:" << p2.getComments().size();
    return 0;
}