#include <gtest/gtest.h>
#include <perst/perst.h>
#include <perst/perst_factory.h>
#include <perst/impl/perst_qsettings.h>
#include <perst/impl/perst_qxml.h>

class TestUsage : public ::testing::Test {
    QCoreApplication * apl;
public:

    TestUsage () {}

    virtual ~TestUsage () {}

    void SetUp(){

        /* prepare to start */
        int argc = 1;
        char * argv[] = {(char*)"perst_testapp"};
        apl = new QCoreApplication (argc, argv);

        /* prepare the ground for easy access to settings */
        QCoreApplication::setOrganizationName("perst_testapp");
        QCoreApplication::setOrganizationDomain("perst_testapp.org");
        QCoreApplication::setApplicationName("perst_testapp");

        PerStFactory::init ();

    }
    void TearDown(){
        PerStFactory::end ();
        delete apl;
        apl = NULL;
    }

};

struct kbForEach_test_data {
    int last_index;
};

bool kbForEach (
        int index, const PERST_STRING & name, PerStFactory::kbCreate kb,
        void * user_data, void * for_each_user_data)
{
    kbForEach_test_data * tdata = (kbForEach_test_data *)for_each_user_data;
    ++tdata->last_index;
    EXPECT_EQ(tdata->last_index, index);

    return true;
}

TEST_F(TestUsage, initial_state) {

    kbForEach_test_data tdata;
    tdata.last_index = -1;
    bool b_res = PerStFactory::forEachProvider (kbForEach, &tdata);
    EXPECT_TRUE(b_res);

}

TEST_F(TestUsage, try_ini) {
    QTemporaryFile tf;
    ASSERT_TRUE(tf.open ());
    QString s_file = tf.fileName ();
    tf.close();
    tf.remove ();

    PerSt * st = PerStFactory::create (PerStQSettings::INI, s_file);
    ASSERT_TRUE(st != NULL);

    EXPECT_TRUE(st->beginGroup ("TOP"));
    EXPECT_TRUE(st->setValue ("test1", "1"));
    EXPECT_TRUE(st->setValue ("test2", "2"));
    EXPECT_TRUE(st->beginGroup ("g1"));
    EXPECT_TRUE(st->setValue ("test1g1", "1"));
    EXPECT_TRUE(st->setValue ("test2g1", "2"));
    EXPECT_TRUE(st->beginWriteArray ("a1"));
        EXPECT_TRUE(st->setValue ("v", "a1v1"));
        EXPECT_TRUE(st->setArrayIndex (1));
        EXPECT_TRUE(st->setValue ("v", "a1v2"));
        EXPECT_TRUE(st->setArrayIndex (2));
        EXPECT_TRUE(st->setValue ("v", "a1v3"));
        EXPECT_TRUE(st->setArrayIndex (3));
        EXPECT_TRUE(st->setValue ("v", "a1v4"));
        EXPECT_TRUE(st->beginWriteArray ("a1a1"));
        EXPECT_TRUE(st->setValue ("v", "a1a1v1"));
        EXPECT_TRUE(st->endArray ("a1a1"));
    EXPECT_TRUE(st->endArray ("a1"));
    EXPECT_TRUE(st->endGroup ("g1"));
    EXPECT_TRUE(st->endGroup ("TOP"));

    delete st;

    st = PerStFactory::create (PerStQSettings::INI, s_file);
    ASSERT_TRUE(st != NULL);

    EXPECT_TRUE(st->beginGroup ("TOP"));
    EXPECT_TRUE(st->valueS ("test1") == "1");
    EXPECT_TRUE(st->valueS ("test2") == "2");
    EXPECT_TRUE(st->beginGroup ("g1"));
    EXPECT_TRUE(st->valueS ("test1g1") == "1");
    EXPECT_TRUE(st->valueS ("test2g1") == "2");
    EXPECT_EQ(st->beginReadArray ("a1"), 4);
        EXPECT_TRUE(st->valueS ("v") == "a1v1");
        EXPECT_TRUE(st->setArrayIndex (1));
        EXPECT_TRUE(st->valueS ("v") == "a1v2");
        EXPECT_TRUE(st->setArrayIndex (2));
        EXPECT_TRUE(st->valueS ("v") == "a1v3");
        EXPECT_TRUE(st->setArrayIndex (3));
        EXPECT_TRUE(st->valueS ("v") == "a1v4");
        EXPECT_EQ(st->beginReadArray ("a1a1"), 1);
        EXPECT_TRUE(st->valueS ("v") == "a1a1v1");
        EXPECT_TRUE(st->endArray ("a1a1"));
    EXPECT_TRUE(st->endArray ("a1"));
    EXPECT_TRUE(st->endGroup ("g1"));
    EXPECT_TRUE(st->endGroup ("TOP"));

    delete st;

    QFile::remove (s_file);
}

TEST_F(TestUsage, try_registry) {
    QTemporaryFile tf;
    ASSERT_TRUE(tf.open ());

    PerSt * st = PerStFactory::create (PerStQSettings::REGS, tf.fileName ());
    ASSERT_TRUE(st != NULL);



    delete st;
}

TEST_F(TestUsage, try_xml) {
    QTemporaryFile tf;
    ASSERT_TRUE(tf.open ());

    PerSt * st = PerStFactory::create (PerStQXml::XML, tf.fileName ());
    ASSERT_TRUE(st != NULL);

    delete st;
}

